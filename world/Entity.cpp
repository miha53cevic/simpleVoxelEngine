#include "Entity.h"

Entity::Entity()
    : position(0, 0, 0)
    , rotation(0, 0, 0)
    , scale(1, 1, 1)
{
}

Entity::~Entity()
{
    for (auto& VBO : VBOs)
        delete VBO;
}

void Entity::setEBO(const std::vector<GLuint>& indicies, int DrawMode)
{
    VAO.Bind();
    EBO.setData(indicies, DrawMode);
    VAO.Unbind();
}

void Entity::setVBO(const std::vector<GLfloat>& data, int attributeID, int size, int DrawMode)
{
    VAO.Bind();
    auto VBO = new gl::VertexBufferObject();
    VBO->setData(data, attributeID, size, DrawMode);
    VBOs.push_back(VBO);
    VAO.Unbind();
}

void Entity::updateEBO(const std::vector<GLuint>& indicies)
{
    VAO.Bind();
    EBO.setSubData(indicies);
    VAO.Unbind();
}

void Entity::updateVBO(const std::vector<GLfloat>& data, int index)
{
    if (index >= VBOs.size())
    {
        printf("Error: VBO at '%d' does not exist!", index);
        return;
    }

    VAO.Bind();
    VBOs[index]->setSubData(data);
    VAO.Unbind();
}
