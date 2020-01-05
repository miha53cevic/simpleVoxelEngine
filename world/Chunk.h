#pragma once
#include <cstdint>

#include "Entity.h"
#include "../Util/Math.h"

namespace Chunk
{
    const int Width = 16, Height = 16, Depth = 16;

    class Chunk
    {
    public:
        Chunk();
        ~Chunk();

        void Update();
        void Render(gl::Shader* shader, gl::Texture* texture, Camera* camera);

        std::uint8_t getBlock(int x, int y, int z);
        void         setBlock(int x, int y, int z, std::uint8_t type);

        void        setPosition(float x, float y, float z);
        glm::vec3   getPosition();

        Entity*      getEntity();

    private:
        std::uint8_t m_blocks[Width][Height][Depth];

        Entity m_entity;

        void generateChunkMesh();
    };
}

