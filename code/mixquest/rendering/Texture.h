//
// MixQuest
// Copyright (c) 2017-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "gameplay/Gameplay.deps.h"
#include <string>
#include <yama/vector2.hpp>
#include "OpenGL.h"

// class for a Texture

class Texture
{
public:
    Texture();
    Texture(const std::string& name);
    ~Texture();

    void loadFromData(GLsizei width, GLsizei height, GLenum format, const void* data);
    bool loadFromFile(const char* path);

    void setParameter(GLenum param, GLint value);

    GLenum type() const { return GL_TEXTURE_2D; }
    GLuint glHandle() const { return m_glHandle; }

    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }
    yama::uvector2 size() const { return {m_width, m_height}; }

    const std::string& name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

private:
    GLuint m_glHandle;
    GLuint m_format; // opengl format of the texture

    uint32_t m_width;
    uint32_t m_height;

    std::string m_name;
};
