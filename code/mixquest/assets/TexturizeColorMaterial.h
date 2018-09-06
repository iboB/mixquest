//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "rendering/GPUProgramPtr.h"
#include "OpenGL.h"
#include <yama/yama.hpp>

class Texture;

// class for texturize material

class TexturizeColorMaterial
{
public:

    void setProjection(const yama::matrix& proj);
    void setTexture(const Texture& tex);

    void begin();
    void end();

    void prepareBuffer(GLuint buffer, uint32_t stride, uint32_t offset);

private:
    friend class LoadAllAssetsState;

    void init();

    GPUProgramPtr m_program;

    GLint m_projLoc = -1;
    GLint m_texLoc = -1;

    GLuint m_vposAttribute = 0;
    GLuint m_texCoordAttribute = 0;
    GLuint m_colorAttribute = 0;
};
