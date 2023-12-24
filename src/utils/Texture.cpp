#define STB_IMAGE_IMPLEMENTATION

#include "Texture.hpp"

#include <print>

#include "stb_image.h"

Texture::Texture(const std::string& path, const std::string& type)
: id(std::make_shared<unsigned int>()), path(path), type(type)
{    
    glGenTextures(1, id.get());
    int width, height, nrComponents;
    
    unsigned char* data=stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if(data){
        GLenum format=0;
        if(nrComponents == 1)
            format = GL_RED;
        else if(nrComponents == 3)
            format = GL_RGB;
        else if(nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, *id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }else{
        std::println("Texture failed to load at path:{}", path);
        stbi_image_free(data);
    }
}

Texture::Texture(const Texture& other)
: id(other.id), path(other.path), type(other.type)
{
}

Texture::~Texture()
{
    if(id.unique())
        glDeleteTextures(1, id.get());
}

void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(GL_TEXTURE_2D, *id);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::flipVertically(bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
}
