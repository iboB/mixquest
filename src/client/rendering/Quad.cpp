//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Quad.h"

#include <maibo/Rendering/Sprites/Sprite.h>

using namespace yama;

namespace
{
point3 Base_Vertices[] =
{
    v(0, 0, 0),
    v(1, 0, 0),
    v(0, 1, 0),
    v(1, 1, 0)
};
}

Quad::Quad()
{
    setPosition(point3::zero());
}

void Quad::setPosition(const yama::vector3& position)
{
    for (int i = 0; i < 4; ++i)
    {
        m_vertices[i].pos = Base_Vertices[i] + position;
    }
}

void Quad::setSprite(const maibo::Sprite& sprite)
{
    m_vertices[0].texCoord = sprite.tc._01;
    m_vertices[1].texCoord = sprite.tc._11;
    m_vertices[2].texCoord = sprite.tc._00;
    m_vertices[3].texCoord = sprite.tc._10;
}
