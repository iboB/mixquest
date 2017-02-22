//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "HealthBar.mix.h"

#include "rendering/Renderer.h"
#include "rendering/Quad.h"

#include "messages/RenderingMessages.h"
#include "gameplay/messages/CharacterMessages.h"
#include "gameplay/messages/TransformMessages.h"

#include <maibo/Rendering/TextureManager.h>

using namespace maibo;

namespace
{
uint32_t Indices[] = { 0, 1, 2, 2, 1, 3 };
}

class HealthBar
{
public:
    void supplyRenderingParts(Renderer& renderer)
    {
        m_quad.setPosition(::position(dm_this).as_vector2_t<float>().xyz(1));

        auto vertices = m_quad.modifyVertices();
        vertices[0].pos.y += 0.9f;
        vertices[1].pos.y += 0.9f;

        float health = float(hp(dm_this)) / float(maxHP(dm_this));
        vertices[1].pos.x -= 1 - health;
        vertices[3].pos.x -= 1 - health;

        Renderer::RenderOp op;
        op.texture = m_texture;
        op.geometry = m_quad.vertices();
        op.numVertices = 4;
        op.indices = Indices;
        op.numIndices = 6;
        renderer.addRenderOp(op);
    }

    void init()
    {
        m_texture = maibo::TextureManager::instance().getTexture("health.png");
        m_quad.setIdentityTexCoords();
    }

private:
    Quad m_quad;
    ConstTexturePtr m_texture;
};

DYNAMIX_DEFINE_MIXIN(HealthBar, supplyRenderingParts_msg);

void initializeHealthBar(dynamix::object& object)
{
    object.get<HealthBar>()->init();
}