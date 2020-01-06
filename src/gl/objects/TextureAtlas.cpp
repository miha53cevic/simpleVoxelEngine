#include "TextureAtlas.h"

// image_size: the width or height of the image (must be 2^x)
// example: 256x256 image, image_size = 256
// individualTexture_size: size of each texture
// example: 64x64 image has 4 textures the size of 32x32, individualTexture_size = 32
gl::TextureAtlas::TextureAtlas(const std::string path, int image_size, int individualTexture_size)
    : TEX_PER_ROW((GLfloat)image_size / (GLfloat)individualTexture_size)
    , INDV_TEX_SIZE(1.0f / TEX_PER_ROW)
    , PIXEL_SIZE(1.0f / (GLfloat)image_size)
{
    texture.loadTexture(path);
}

// get the coordinates of the texture at given position in the 2D grid
std::vector<GLfloat> gl::TextureAtlas::getTextureCoords(const glm::ivec2 & coords)
{
    GLfloat xMin = (coords.x * INDV_TEX_SIZE) + 0.5f * PIXEL_SIZE;
    GLfloat yMin = (coords.y * INDV_TEX_SIZE) + 0.5f * PIXEL_SIZE;

    GLfloat xMax = (xMin + INDV_TEX_SIZE) - PIXEL_SIZE;
    GLfloat yMax = (yMin + INDV_TEX_SIZE) - PIXEL_SIZE;

    return { xMin, yMin, xMin, yMax, xMax, yMax, xMax, yMin };
}
