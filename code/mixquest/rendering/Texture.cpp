//
// MixQuest
// Copyright (c) 2017-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Texture.h"

#include "Util.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STB_IMAGE_STATIC
#include "stb_image.h"

using namespace std;

Texture::Texture()
    : Texture("<unnamed texture>")
{
}

Texture::Texture(const std::string& name)
    : m_width(0)
    , m_height(0)
    , m_name(name)
{
    glGenTextures(1, &m_glHandle);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_glHandle);
}

void Texture::loadFromData(GLsizei width, GLsizei height, GLenum format, const void* data)
{
    glBindTexture(GL_TEXTURE_2D, m_glHandle);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    m_format = format;
    m_width = width;
    m_height = height;
}

bool Texture::loadFromFile(const char* path)
{
    vector<uint8_t> data;
    if (Util::ReadFileToVector(path, data)) return false;

    int w, h, numChannels;
    auto img = stbi_load_from_memory(data.data(), int(data.size()), &w, &h, &numChannels, 0);

    if (!img) return false;

    GLenum format;
    switch (numChannels)
    {
    case 1: format = GL_LUMINANCE; break;
    case 2: format = GL_LUMINANCE_ALPHA; break;
    case 3: format = GL_RGB; break;
    case 4:
    default:
        format = GL_RGBA; break;
    }

    loadFromData(w, h, format, img);

    stbi_image_free(img);

    return true;

}

void Texture::setParameter(GLenum param, GLint value)
{
    glTexParameteri(GL_TEXTURE_2D, param, value);
}
