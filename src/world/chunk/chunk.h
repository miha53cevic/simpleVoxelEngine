#pragma once
#include <cstdint>

#include "../Entity.h"
#include "../../util/math/Math.h"

class ChunkManager;

namespace Chunk
{
    const int Width = 16, Height = 32, Depth = 16;

    class Chunk
    {
    public:
        Chunk(gl::TextureAtlas* atlas, ChunkManager* manager);
        ~Chunk();

        void Update();
        void Render(gl::Shader* shader, gl::Texture* texture, Camera* camera);

        void generateTerrain(float freq = 10, float minAmp = 1, float maxAmp = Height);

        std::uint8_t getBlock(int x, int y, int z);
        void         setBlock(int x, int y, int z, std::uint8_t type);

        void         setPosition(float x, float y, float z);
        glm::vec3    getPosition();

        Entity*      getEntity();

    private:
        std::uint8_t m_blocks[Width][Height][Depth];

        Entity m_entity;

        gl::TextureAtlas* m_textureAtlas;
        ChunkManager*     m_chunkManager;

        void generateChunkMesh();
        void generateTrees(std::vector< glm::ivec3> treeLocations);
    };
}

