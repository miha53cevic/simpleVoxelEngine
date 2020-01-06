#pragma once
#include <glad/glad.h>

namespace gl
{
    struct VertexArray
    {
        VertexArray();
        ~VertexArray();

        void Bind();
        void Unbind();

        GLuint VAO;
    };
};
