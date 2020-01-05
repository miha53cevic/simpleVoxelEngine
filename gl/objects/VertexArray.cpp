#include "VertexArray.h"

gl::VertexArray::VertexArray()
{
    glGenVertexArrays(1, &VAO);
}

gl::VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &VAO);
}

void gl::VertexArray::Bind()
{
    glBindVertexArray(VAO);
}

void gl::VertexArray::Unbind()
{
    glBindVertexArray(0);
}
