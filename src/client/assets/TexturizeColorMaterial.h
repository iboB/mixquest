//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/Rendering/GPUProgramPtr.h>

namespace maibo
{
class Texture;
}

// class for texturize material

class TexturizeColorMaterial
{
public:

    void setProjection(const yama::matrix& proj);
    void setTexture(const maibo::Texture& tex);

    void begin();
    void end();

    void prepareBuffer(GLuint buffer, uint32_t stride, uint32_t offset);

private:
    friend class LoadAllAssetsState;

    void init();

    maibo::GPUProgramPtr m_program;

    GLint m_projLoc = -1;
    GLint m_texLoc = -1;

    uint32_t m_vposAttribute;
    uint32_t m_texCoordAttribute;
    uint32_t m_colorAttribute;
};
