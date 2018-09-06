//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "TexturizeColorMaterial.h"

#include "rendering/Texture.h"
#include "rendering/GPUProgram.h"
#include "Memory.h"

void TexturizeColorMaterial::setProjection(const yama::matrix& proj)
{
    m_program->setParameter(m_projLoc, proj);
}

void TexturizeColorMaterial::setTexture(const Texture& tex)
{
    m_program->resetUniforms();
    m_program->setParameter(m_texLoc, tex);
}

void TexturizeColorMaterial::begin()
{
    m_program->use();
    glEnableVertexAttribArray(m_vposAttribute);
    glEnableVertexAttribArray(m_texCoordAttribute);
    glEnableVertexAttribArray(m_colorAttribute);
}

void TexturizeColorMaterial::end()
{
    glDisableVertexAttribArray(m_vposAttribute);
    glDisableVertexAttribArray(m_texCoordAttribute);
    glDisableVertexAttribArray(m_colorAttribute);
}

void TexturizeColorMaterial::prepareBuffer(GLuint buffer, uint32_t stride, uint32_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(m_vposAttribute, 3, GL_FLOAT, GL_FALSE, stride, OFFSET(offset));
    glVertexAttribPointer(m_texCoordAttribute, 2, GL_FLOAT, GL_FALSE, stride, OFFSET(offset + sizeof(yama::vector3)));
    glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, stride, OFFSET(offset + sizeof(yama::vector3) + sizeof(yama::vector2)));
}

void TexturizeColorMaterial::init()
{
    assert(m_program);

    m_projLoc = m_program->getParameterByName("u_proj");
    m_texLoc = m_program->getParameterByName("u_tex");

    m_vposAttribute = GLuint(m_program->getAttributeLocation("a_pos"));
    m_texCoordAttribute = GLuint(m_program->getAttributeLocation("a_texCoord"));
    m_colorAttribute = GLuint(m_program->getAttributeLocation("a_color"));
}
