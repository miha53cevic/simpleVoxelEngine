#include "ElementArrayBuffer.h"

#include <stdio.h>

gl::ElementArrayBuffer::ElementArrayBuffer()
{
    glGenBuffers(1, &EBO);
}

gl::ElementArrayBuffer::~ElementArrayBuffer()
{
    glDeleteBuffers(1, &EBO);
}

void gl::ElementArrayBuffer::setData(const std::vector<GLuint>& indicies, int DrawMode)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies.size(), indicies.data(), DrawMode);

    size = indicies.size();
}

void gl::ElementArrayBuffer::setSubData(const std::vector<GLuint>& indicies)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * indicies.size(), indicies.data());

    size = indicies.size();
}
