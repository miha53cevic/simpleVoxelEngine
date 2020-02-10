#pragma once
#include <vector>
#include <glad/glad.h>

namespace Cube
{
    struct Face
    {
        std::vector<GLfloat> verticies;
		std::vector<GLfloat> normals;

        // Indicies and textureCoords of each face are the same
        std::vector<GLuint>  indicies = {
            0,1,3,
            3,1,2
        };
        std::vector<GLfloat> textureCoords = {
            0,0,
            0,1,
            1,1,
            1,0
        };
    };

    enum class CubeFace
    {
        TOP, BOTTOM, LEFT, RIGHT, BACK, FRONT
    };

    static Face getCubeFace(CubeFace face)
    {
        if (face == CubeFace::BACK)
            return Face{
                {   // Verticies
                    1,1,0,
                    1,0,0,
                    0,0,0,
                    0,1,0
                },
                {   // Normals
                    0,0,-1
                }
        };
        else if (face == CubeFace::FRONT)
            return Face{
                {
                    0,1,1,
                    0,0,1,
                    1,0,1,
                    1,1,1
                },
                {
                    0,0,1
                }
        };
        else if (face == CubeFace::RIGHT)
            return Face{
                {
                    1,1,1,
                    1,0,1,
                    1,0,0,
                    1,1,0
                },
                {
                    1,0,0
                }
        };
        else if (face == CubeFace::LEFT)
            return Face{
                {
                    0,1,0,
                    0,0,0,
                    0,0,1,
                    0,1,1
                },
                {
                    -1,0,0
                }
        };
        else if (face == CubeFace::TOP)
            return Face{
                {
                    0,1,1,
                    1,1,1,
                    1,1,0,
                    0,1,0
                },
                {
                    0,1,0
                }
        };
        else if (face == CubeFace::BOTTOM)
            return Face{
                {
                    0,0,1,
                    0,0,0,
                    1,0,0,
                    1,0,1
                },
                {
                    0,-1,0
                }
        };
    }
};
