#include "Texture.h"

Texture::Texture()
{
    textureId = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = "";
}

Texture::Texture(const char* path)
{
    textureId = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = path;
}

void Texture::LoadTexture()
{
    unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

    if (!texData)
    {
        std::cout << "La carga de textura ha fallado: " << fileLocation << std::endl;
        return;
    }

    std::cout << "Textura cargada: " << fileLocation
        << " | Width: " << width
        << " | Height: " << height
        << " | Canales: " << bitDepth
        << std::endl;

    GLenum format = GL_RGB;
    GLenum internalFormat = GL_RGB;

    if (bitDepth == 1)
    {
        format = GL_RED;
        internalFormat = GL_RED;
    }
    else if (bitDepth == 3)
    {
        format = GL_RGB;
        internalFormat = GL_RGB;
    }
    else if (bitDepth == 4)
    {
        format = GL_RGBA;
        internalFormat = GL_RGBA;
    }
    else
    {
        std::cout << "Formato de textura no soportado: " << bitDepth << " canales" << std::endl;
        stbi_image_free(texData);
        return;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        texData
    );

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(texData);
}

void Texture::UseTexture(int textureNum)
{
    glActiveTexture(GL_TEXTURE0 + textureNum);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::ClearTexture()
{
    if (textureId != 0)
    {
        glDeleteTextures(1, &textureId);
        textureId = 0;
    }

    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = "";
}

Texture::~Texture()
{
    ClearTexture();
}