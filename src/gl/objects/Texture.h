#pragma once
#include <string>

#include <SFML/Graphics.hpp>
#include "glad/glad.h"

namespace gl
{
    struct Texture
    {
        Texture();
        Texture(std::string texture_path);
        ~Texture();

        void loadTexture(std::string texture_path);

        GLuint texture;
    };
};