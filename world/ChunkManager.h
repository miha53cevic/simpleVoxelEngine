#pragma once
#include "Chunk.h"

class ChunkManager
{
public:
    ChunkManager();
    ~ChunkManager();

    void generate(int width, int height, int depth, gl::TextureAtlas* atlas);
    void render(gl::Shader* shader, gl::Texture* texture, Camera* camera);

    std::uint8_t  getBlock(int x, int y, int z);
    void          setBlock(int x, int y, int z, std::uint8_t type);

    Chunk::Chunk* getChunkFromWorldPosition(int x, int y, int z);
    Chunk::Chunk* getChunkFromArray        (int x, int y, int z);

    glm::uvec3 getChunksSize();

private:
    std::vector<Chunk::Chunk*> m_chunks;

    glm::uvec3 m_chunksSize;

    int Convert3Dto1D(float x, float y, float z);
};

