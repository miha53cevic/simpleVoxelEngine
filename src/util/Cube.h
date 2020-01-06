#pragma once
#include <vector>
#include <glad/glad.h>

namespace Cube
{
    struct Face
    {
        std::vector<GLfloat> verticies;

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

    static enum class CubeFace
    {
        TOP, BOTTOM, LEFT, RIGHT, BACK, FRONT
    };

    static Face getCubeFace(CubeFace face)
    {
        if (face == CubeFace::BACK)
            return Face {
                {
                    1,1,0,
                    1,0,0,
                    0,0,0,
                    0,1,0
                },
            };
        else if (face == CubeFace::FRONT)
            return Face{
                {
                    0,1,1,
                    0,0,1,
                    1,0,1,
                    1,1,1
                }
            };
        else if (face == CubeFace::RIGHT)
            return Face{
                {
                    1,1,1,
                    1,0,1,
                    1,0,0,
                    1,1,0
                }
            };
        else if (face == CubeFace::LEFT)
            return Face{
                {
                    0,1,0,
                    0,0,0,
                    0,0,1,
                    0,1,1
                }
            };
        else if (face == CubeFace::TOP)
            return Face{
                {
                    0,1,1,
                    1,1,1,
                    1,1,0,
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
                }
            };
    }

    static std::vector<GLfloat> verticies = {
        // Back face
        1,1,0,
        1,0,0,
        0,0,0,
        0,1,0,

        // Front face
        0,1,1,
        0,0,1,
        1,0,1,
        1,1,1,

        // Right face
        1,1,1,
        1,0,1,
        1,0,0,
        1,1,0,

        // Left Face
        0,1,0,
        0,0,0,
        0,0,1,
        0,1,1,

        // Top face
        0,1,1,
        1,1,1,
        1,1,0,
        0,1,0,

        // Bottom face
        0,0,1,
        0,0,0,
        1,0,0,
        1,0,1
    };

    static std::vector<GLfloat> textureCoords = {
         0,0,
         0,1,
         1,1,
         1,0,

         0,0,
         0,1,
         1,1,
         1,0,

         0,0,
         0,1,
         1,1,
         1,0,

         0,0,
         0,1,
         1,1,
         1,0,

         0,0,
         0,1,
         1,1,
         1,0,

         0,0,
         0,1,
         1,1,
         1,0
     };

    static std::vector<GLuint> indicies = {
         0,1,3,
         3,1,2,
         4,5,7,
         7,5,6,
         8,9,11,
         11,9,10,
         12,13,15,
         15,13,14,
         16,17,19,
         19,17,18,
         20,21,23,
         23,21,22
     };
};
