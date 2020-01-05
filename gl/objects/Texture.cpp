#include "Texture.h"

gl::Texture::Texture()
{
}

gl::Texture::Texture(std::string texture_path)
{
    loadTexture(texture_path);
}

gl::Texture::~Texture()
{
    glDeleteTextures(1, &texture);
}

void gl::Texture::loadTexture(std::string texture_path)
{
    // Load the texture
    sf::Image img;
    if (!img.loadFromFile(texture_path))
    {
        // Error
        texture = -1;
    }
    else
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Send texture data to the GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

        // Must add these otherwise the texture doesn't load
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
}
