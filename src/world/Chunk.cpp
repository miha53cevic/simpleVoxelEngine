#include "Chunk.h"

#include "../util/Cube.h"
#include "ChunkManager.h"
#include "BlockManager.h"

Chunk::Chunk::Chunk(gl::TextureAtlas * atlas, ChunkManager* manager)
    : m_textureAtlas(atlas)
    , m_chunkManager(manager)
{
    // Fill in the chunk with nothing at first
    for (int x = 0; x < Width; x++)
        for (int y = 0; y < Height; y++)
            for (int z = 0; z < Depth; z++)
                m_blocks[x][y][z] = 0;
}

Chunk::Chunk::~Chunk()
{
}

void Chunk::Chunk::Update()
{
    generateChunkMesh();
}

void Chunk::Chunk::Render(gl::Shader * shader, gl::Texture * texture, Camera * camera)
{
    shader->Bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->texture);

    shader->loadMatrix(shader->getUniformLocation("viewMatrix"), Math::createViewMatrix(camera));

    m_entity.VAO.Bind();

    shader->loadMatrix(shader->getUniformLocation("transformationMatrix"), Math::createTransformationMatrix(m_entity.position, m_entity.rotation, m_entity.scale));

    glDrawElements(GL_TRIANGLES, m_entity.EBO.size, GL_UNSIGNED_INT, 0);
    m_entity.VAO.Unbind();

    shader->Unbind();
}

void Chunk::Chunk::generateTerrain(float freq, float minAmp, float maxAmp)
{
    std::vector< glm::ivec3> treeLocations;

    // Fill in the chunk with blocks
    for (int x = 0; x < Width; x++)
    {
        for (int z = 0; z < Depth; z++)
        {
            int posX = m_entity.position.x;
            int posZ = m_entity.position.z;

            int height = Math::simplex2((posX + x) / freq, (posZ + z) / freq, 4, 1.5f, 0.1f) * maxAmp + minAmp;

            for (int y = 0; y < height - 1; y++)
            {
                if (height >= 3 && y < height - 3) m_blocks[x][y][z] = BLOCK::STONE;
                else                               m_blocks[x][y][z] = BLOCK::DIRT;
            }

            // Top layer is grass
            m_blocks[x][height - 1][z] = BLOCK::GRASS;

            // Save random tree locations
            float rand = Math::fRandom(0, 1);
            if (x == 0 || x == Width - 1 || z == 0 || z == Depth - 1) continue;
            else if (rand >= 0.99) treeLocations.push_back({ x, height, z });
        }
    }

    // Add trees to the world
    generateTrees(treeLocations);

    // Generate the mesh
    generateChunkMesh();
}

std::uint8_t Chunk::Chunk::getBlock(int x, int y, int z)
{
    return m_blocks[x][y][z];
}

void Chunk::Chunk::setBlock(int x, int y, int z, std::uint8_t type)
{
    m_blocks[x][y][z] = type;
}

void Chunk::Chunk::setPosition(float x, float y, float z)
{
    m_entity.position = glm::vec3(x, y, z);
}

glm::vec3 Chunk::Chunk::getPosition()
{
    return m_entity.position;
}

Entity * Chunk::Chunk::getEntity()
{
    return &m_entity;
}

void Chunk::Chunk::generateChunkMesh()
{
    // Generate mesh by only drawing the faces if there aren't any blocks next to them
    std::vector<GLfloat> temp_verticies;
    std::vector<GLfloat> temp_normals;
    std::vector<GLuint> temp_indicies;
    std::vector<GLfloat> temp_textureCoords;
    int indicieCount = 0;

    for (int x = 0; x < Width; x++)
    {
        for (int y = 0; y < Height; y++)
        {
            for (int z = 0; z < Depth; z++)
            {
                // Create block face data
                auto createFace = [&](Cube::Face face, Cube::CubeFace face_name, int x, int y, int z)
                {
                    for (int i = 0; i < face.verticies.size() / 3; i++)
                    {
                        temp_verticies.push_back(face.verticies[0 + i * 3] + x);
                        temp_verticies.push_back(face.verticies[1 + i * 3] + y);
                        temp_verticies.push_back(face.verticies[2 + i * 3] + z);
                    }

                    for (int i = 0; i < 3; i++)
                    {
                        temp_normals.push_back(face.normals[0]);
                        temp_normals.push_back(face.normals[1]);
                        temp_normals.push_back(face.normals[2]);
                    }

                    // Use the texture atlas for each face
                    glm::ivec2 textureIndex = BlockManager::getTextureAtlasCoords((BLOCK)m_blocks[x][y][z], face_name);

                    auto texCoords = m_textureAtlas->getTextureCoords(textureIndex);
                    temp_textureCoords.insert(temp_textureCoords.end(), texCoords.begin(), texCoords.end());

                    temp_indicies.push_back(indicieCount);
                    temp_indicies.push_back(indicieCount + 1);
                    temp_indicies.push_back(indicieCount + 3);
                    temp_indicies.push_back(indicieCount + 3);
                    temp_indicies.push_back(indicieCount + 1);
                    temp_indicies.push_back(indicieCount + 2);

                    indicieCount += 4;
                };

                // Check if the block is Air, if it is no need to draw it
                if (!m_blocks[x][y][z])
                    continue;

                // Draw the faces on the end of the chunk
                if (x - 1 < 0)       createFace(Cube::getCubeFace(Cube::CubeFace::LEFT), Cube::CubeFace::LEFT, x, y, z);
                if (x + 1 >= Width)  createFace(Cube::getCubeFace(Cube::CubeFace::RIGHT), Cube::CubeFace::RIGHT, x, y, z);
                // no need to draw the bottom layer since it is never seen
                if (y + 1 >= Height) createFace(Cube::getCubeFace(Cube::CubeFace::TOP), Cube::CubeFace::TOP, x, y, z);
                if (z - 1 < 0)       createFace(Cube::getCubeFace(Cube::CubeFace::BACK), Cube::CubeFace::BACK, x, y, z);
                if (z + 1 >= Depth)  createFace(Cube::getCubeFace(Cube::CubeFace::FRONT), Cube::CubeFace::FRONT, x, y, z);

                // Draw faces only if the neighbouring block is air
                if (x - 1 >= 0 && !m_blocks[x - 1][y][z])     createFace(Cube::getCubeFace(Cube::CubeFace::LEFT), Cube::CubeFace::LEFT, x, y, z);
                if (x + 1 < Width && !m_blocks[x + 1][y][z])  createFace(Cube::getCubeFace(Cube::CubeFace::RIGHT), Cube::CubeFace::RIGHT, x, y, z);
                if (y - 1 >= 0 && !m_blocks[x][y - 1][z])     createFace(Cube::getCubeFace(Cube::CubeFace::BOTTOM), Cube::CubeFace::BOTTOM, x, y, z);
                if (y + 1 < Height && !m_blocks[x][y + 1][z]) createFace(Cube::getCubeFace(Cube::CubeFace::TOP), Cube::CubeFace::TOP, x, y, z);
                if (z - 1 >= 0 && !m_blocks[x][y][z - 1])     createFace(Cube::getCubeFace(Cube::CubeFace::BACK), Cube::CubeFace::BACK, x, y, z);
                if (z + 1 < Depth && !m_blocks[x][y][z + 1])  createFace(Cube::getCubeFace(Cube::CubeFace::FRONT), Cube::CubeFace::FRONT, x, y, z);
            }
        }
    }

    // Fill in the entity
    m_entity.setEBO(temp_indicies, GL_DYNAMIC_DRAW);
    m_entity.setVBO(temp_verticies, 0, 3, GL_DYNAMIC_DRAW);
    m_entity.setVBO(temp_textureCoords, 1, 2, GL_DYNAMIC_DRAW);
    m_entity.setVBO(temp_textureCoords, 2, 3, GL_DYNAMIC_DRAW);
}

void Chunk::Chunk::generateTrees(std::vector< glm::ivec3> treeLocations)
{
    for (auto& treeLoc : treeLocations)
    {
        int treeHeight = Math::iRandom(4, 8);
        for (int i = 0; i < treeHeight && treeLoc.y + (treeHeight - 1) < Height; i++)
            m_blocks[treeLoc.x][treeLoc.y + i][treeLoc.z] = BLOCK::WOOD;

        for (int x = -1; x <= 1; x++)
        {
            for (int y = 0; y < 3; y++)
            {
                for (int z = -1; z <= 1; z++)
                {
                    if (x == 0 && z == 0 && y != 2) continue;
                    else m_blocks[treeLoc.x + x][treeLoc.y + y + (treeHeight - 3)][treeLoc.z + z] = BLOCK::LEAF;
                }
            }
        }
    }
}
