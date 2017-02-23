//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "SelectionMark.mix.h"

#include "rendering/Renderer.h"
#include "rendering/Quad.h"

#include "messages/RenderingMessages.h"
#include "gameplay/messages/CharacterMessages.h"
#include "gameplay/messages/TransformMessages.h"

#include <maibo/Rendering/Sprites/Tileset.h>
#include <maibo/Rendering/TextureManager.h>

using namespace maibo;
using namespace yama;

namespace
{
uint32_t Indices[] = { 0, 1, 2, 2, 1, 3 };
}

class SelectionMark
{
public:
    void supplyRenderingParts(Renderer& renderer)
    {
        if (!m_tileset.texture()) init();

        m_quad.setSprite(m_tileset.getTile(::team(dm_this)));
        m_quad.setPosition(::position(dm_this).as_vector2_t<float>().xyz(1));

        auto v = m_quad.modifyVertices();
        for (int i = 0; i < 4; ++i)
        {
            v[i].pos.y -= 0.2f;
        }

        Renderer::RenderOp op;
        op.texture = m_tileset.texture();;
        op.geometry = m_quad.vertices();
        op.numVertices = 4;
        op.indices = Indices;
        op.numIndices = 6;
        renderer.addRenderOp(op);
    }

    void init()
    {
        m_tileset.setTexture(maibo::TextureManager::instance().getTexture("selection.png"));
        m_tileset.setTileSize(vt(32u, 32u));
    }

private:
    Quad m_quad;
    Tileset m_tileset;
};

DYNAMIX_DEFINE_MIXIN(SelectionMark, priority(1, supplyRenderingParts_msg));
