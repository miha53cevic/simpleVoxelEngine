#pragma once
#include <glad/glad.h>

#include <vector>

namespace gl
{
    struct ElementArrayBuffer
    {
        ElementArrayBuffer();
        ~ElementArrayBuffer();

        void setData   (const std::vector<GLuint>& indicies, int DrawMode = GL_STATIC_DRAW);
        void setSubData(const std::vector<GLuint>& indicies);

        GLuint EBO;
        GLuint size;
    };
};

