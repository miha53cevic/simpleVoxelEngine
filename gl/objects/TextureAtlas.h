#pragma once
#include <glm/glm.hpp>

#include "Texture.h"

namespace gl
{
    struct TextureAtlas
    {
        TextureAtlas(const std::string path, int image_size, int individualTexture_size);

        std::vector<GLfloat> getTextureCoords(const glm::ivec2& coords);

        Texture texture;

        const GLfloat TEX_PER_ROW;
        const GLfloat INDV_TEX_SIZE;
        const GLfloat PIXEL_SIZE;
    };
};

