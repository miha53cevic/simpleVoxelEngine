#pragma once
#include "glm/glm.hpp"

#include "../util/Cube.h"

enum BLOCK
{
    AIR   = 0,
    GRASS = 1,
    DIRT  = 2,
    STONE = 3,
    WOOD  = 4,
    LEAF  = 5
};

namespace BlockManager
{
    static glm::ivec2 getTextureAtlasCoords(BLOCK type, Cube::CubeFace face)
    {
        switch (type)
        {
        case BLOCK::GRASS:
            if (face == Cube::CubeFace::TOP)
                return glm::ivec2(0, 0);
            if (face == Cube::CubeFace::BOTTOM)
                return glm::ivec2(2, 0);
            if (face == Cube::CubeFace::LEFT || face == Cube::CubeFace::RIGHT || face == Cube::CubeFace::FRONT || face == Cube::CubeFace::BACK)
                return glm::ivec2(1, 0);

            break;

        case BLOCK::DIRT:
            return glm::ivec2(2, 0);
            
            break;

        case BLOCK::STONE:
            return glm::ivec2(3, 0);

            break;
        }

        return glm::ivec2(0, 0);
    }
};