//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "Vertex.h"

#include <yama/yama.hpp>

struct Sprite;

class Quad
{
public:
    Quad();

    void setPosition(const yama::vector3& position);

    void setSprite(const Sprite& sprite);

    void setIdentityTexCoords();

    const Vertex* vertices() const {
        return m_vertices;
    }

    Vertex* modifyVertices() {
        return m_vertices;
    }

private:

    Vertex m_vertices[4];

};
