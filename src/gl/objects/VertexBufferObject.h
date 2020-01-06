#pragma once
#include <glad/glad.h>
#include <vector>

namespace gl
{
    struct VertexBufferObject
    {
        VertexBufferObject();
        ~VertexBufferObject();

        GLuint VBO;

        void setData   (const std::vector<GLfloat>& data, int attributeID, int size, int DrawMode = GL_STATIC_DRAW);
        void setSubData(const std::vector<GLfloat>& data);
    };
};
