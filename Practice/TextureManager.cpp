#include "TextureManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



TextureManager* TextureManager::sInstance = nullptr;
TextureManager& TextureManager::Instance()
{
    //assert if null
    return *sInstance;
}

void TextureManager::Create()
{
    if (!sInstance)
    {
        sInstance = new TextureManager();
    }
}

void TextureManager::Destroy()
{
    if (sInstance)
    {
        delete sInstance;
        sInstance = nullptr;
    }
}

bool TextureManager::IsValid()
{
    return sInstance != nullptr;
}

unsigned int TextureManager::LoadFile(const char* fileName, bool hasAlphaChannel)
{
    // load and create a texture 
    // -------------------------
    unsigned int textureId;
    // texture 1
    // ---------
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
    if (data)
    {
        if (hasAlphaChannel)
        {
            // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return textureId;
}