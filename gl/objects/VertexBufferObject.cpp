#include "VertexBufferObject.h"

gl::VertexBufferObject::VertexBufferObject()
{
    glGenBuffers(1, &VBO);
}

gl::VertexBufferObject::~VertexBufferObject()
{
    glDeleteBuffers(1, &VBO);
    printf("VBO: Deconstructor called\n");
}

void gl::VertexBufferObject::setData(const std::vector<GLfloat>& data, int attributeID, int size, int DrawMode)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), DrawMode);

    glEnableVertexAttribArray(attributeID);
    glVertexAttribPointer(attributeID, size, GL_FLOAT, false, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void gl::VertexBufferObject::setSubData(const std::vector<GLfloat>& data)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * data.size(), data.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}