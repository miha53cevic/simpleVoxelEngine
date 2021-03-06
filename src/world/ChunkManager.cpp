#include "ChunkManager.h"

ChunkManager::ChunkManager()
{
}

ChunkManager::~ChunkManager()
{
    for (auto& chunk : m_chunks)
        delete chunk;
}

void ChunkManager::generate(int width, int height, int depth, gl::TextureAtlas* atlas)
{
    m_chunksSize = glm::uvec3(width, height, depth);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int z = 0; z < depth; z++)
            {
                // Create the chunks
                auto* temp_chunk = new Chunk::Chunk(atlas, this);
                temp_chunk->setPosition(x * Chunk::Width, y * Chunk::Height, z * Chunk::Depth);

                m_chunks.push_back(temp_chunk);
            }
        }
    }

    // Generate terrain for each chunk
    for (auto& chunk : m_chunks)
        chunk->generateTerrain(24, 8, Chunk::Height - 8);
}

void ChunkManager::render(gl::Shader * shader, gl::Texture * texture, Camera * camera)
{
    for (auto& chunk : m_chunks)
        chunk->Render(shader, texture, camera);
}

std::uint8_t ChunkManager::getBlock(int x, int y, int z)
{
    // First get which chunk it is
    int dx = x / Chunk::Width;
    int dy = y / Chunk::Height;
    int dz = z / Chunk::Depth;

    int index = Convert3Dto1D(dx, dy, dz);
    auto chunk = m_chunks.at(index);

    // Then calculate which block it is
    int sx = x % Chunk::Width;
    int sy = y % Chunk::Height;
    int sz = z % Chunk::Depth;
    
    return chunk->getBlock(sx, sy, sz);
}

void ChunkManager::setBlock(int x, int y, int z, std::uint8_t type)
{
    // First get which chunk it is
    int dx = x / Chunk::Width;
    int dy = y / Chunk::Height;
    int dz = z / Chunk::Depth;

    int index = Convert3Dto1D(dx, dy, dz);
    auto chunk = m_chunks.at(index);

    // Then calculate which block it is
    int sx = x % Chunk::Width;
    int sy = y % Chunk::Height;
    int sz = z % Chunk::Depth;

    chunk->setBlock(sx, sy, sz, type);
}

Chunk::Chunk * ChunkManager::getChunkFromWorldPosition(int x, int y, int z)
{
    int dx = x / Chunk::Width;
    int dy = y / Chunk::Height;
    int dz = z / Chunk::Depth;

    int index = Convert3Dto1D(dx, dy, dz);
    return m_chunks.at(index);
}

Chunk::Chunk * ChunkManager::getChunkFromArray(int x, int y, int z)
{
    int index = Convert3Dto1D(x, y, z);
    return m_chunks.at(index);
}

bool ChunkManager::hasNeighbour(Chunk::Chunk * chunk, Direction dir)
{
    glm::vec3 pos = chunk->getEntity()->position;
    
    if (dir == Direction::WEST && (pos.x - 1)  / Chunk::Width >= 0)                        return true;
    if (dir == Direction::EAST && (pos.x + Chunk::Width)  / Chunk::Width < m_chunksSize.x) return true;
    if (dir == Direction::NORTH && (pos.z - 1) / Chunk::Depth >= 0)                        return true;
    if (dir == Direction::SOUTH && (pos.z + Chunk::Depth) / Chunk::Depth < m_chunksSize.z) return true;

    return false;
}

glm::uvec3 ChunkManager::getChunksSize()
{
    return m_chunksSize;
}

int ChunkManager::Convert3Dto1D(float x, float y, float z)
{
    // Index = ((x * YSIZE + y) * ZSIZE) + z;
    int WIDTH  = m_chunksSize.x;
    int HEIGHT = m_chunksSize.y;
    int DEPTH  = m_chunksSize.z;

    return ((x * HEIGHT + y) * DEPTH) + z;
}
