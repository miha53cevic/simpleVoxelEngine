#include "Chunk.h"

#include "../Util/Cube.h"
#include "BlockManager.h"

#include "glm/gtc/noise.hpp"

Chunk::Chunk::Chunk(gl::TextureAtlas * atlas)
    : m_textureAtlas(atlas)
{
    // Fill in the chunk with nothing at first
    for (int x = 0; x < Width; x++)
        for (int y = 0; y < Height; y++)
            for (int z = 0; z < Depth; z++)
                m_blocks[x][y][z] = 0;

    // Generate the mesh
    generateChunkMesh();
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
    auto getNoise = [&](float x, float z) -> int
    {
        return abs(glm::perlin(glm::vec2(x / freq, z / freq)) * maxAmp) + minAmp;
    };

    // Fill in the chunk with blocks
    for (int x = 0; x < Width; x++)
    {
        for (int z = 0; z < Depth; z++)
        {
            int posX = m_entity.position.x;
            int posZ = m_entity.position.z;

            int height = getNoise(posX + x, posZ + z);

            for (int y = 0; y < height - 1; y++)
            {
                if (height >= 3 && y < height - 3) m_blocks[x][y][z] = BLOCK::STONE;
                else                               m_blocks[x][y][z] = BLOCK::DIRT;
            }

            // Top layer is grass
            m_blocks[x][height - 1][z] = BLOCK::GRASS;
        }
    }

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
                if (x - 1 < 0)       createFace(Cube::getCubeFace(Cube::CubeFace::LEFT  ), Cube::CubeFace::LEFT,   x, y, z);
                if (x + 1 >= Width)  createFace(Cube::getCubeFace(Cube::CubeFace::RIGHT ), Cube::CubeFace::RIGHT,  x, y, z);
                // no need to draw the bottom layer since it is never seen
                if (y + 1 >= Height) createFace(Cube::getCubeFace(Cube::CubeFace::TOP   ), Cube::CubeFace::TOP,    x, y, z);
                if (z - 1 < 0)       createFace(Cube::getCubeFace(Cube::CubeFace::BACK  ), Cube::CubeFace::BACK,   x, y, z);
                if (z + 1 >= Depth)  createFace(Cube::getCubeFace(Cube::CubeFace::FRONT ), Cube::CubeFace::FRONT,  x, y, z);

                // Draw faces only if the neighbouring block is air
                if (x - 1 >= 0 && !m_blocks[x - 1][y][z])     createFace(Cube::getCubeFace(Cube::CubeFace::LEFT),   Cube::CubeFace::LEFT,   x, y, z);
                if (x + 1 < Width && !m_blocks[x + 1][y][z])  createFace(Cube::getCubeFace(Cube::CubeFace::RIGHT),  Cube::CubeFace::RIGHT,  x, y, z);
                if (y - 1 >= 0 && !m_blocks[x][y - 1][z])     createFace(Cube::getCubeFace(Cube::CubeFace::BOTTOM), Cube::CubeFace::BOTTOM, x, y, z);
                if (y + 1 < Height && !m_blocks[x][y + 1][z]) createFace(Cube::getCubeFace(Cube::CubeFace::TOP),    Cube::CubeFace::TOP,    x, y, z);
                if (z - 1 >= 0 && !m_blocks[x][y][z - 1])     createFace(Cube::getCubeFace(Cube::CubeFace::BACK),   Cube::CubeFace::BACK,   x, y, z);
                if (z + 1 < Depth && !m_blocks[x][y][z + 1])  createFace(Cube::getCubeFace(Cube::CubeFace::FRONT),  Cube::CubeFace::FRONT,  x, y, z);
            }
        }
    }

    // Fill in the entity
    m_entity.setEBO(temp_indicies, GL_DYNAMIC_DRAW);
    m_entity.setVBO(temp_verticies, 0, 3, GL_DYNAMIC_DRAW);
    m_entity.setVBO(temp_textureCoords, 1, 2, GL_DYNAMIC_DRAW);
}
