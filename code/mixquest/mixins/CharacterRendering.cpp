//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "CharacterRendering.mix.h"

#include "assets/GlobalAssets.h"

#include "messages/RenderingMessages.h"

#include "gameplay/messages/CharacterMessages.h"
#include "gameplay/messages/TransformMessages.h"

#include "rendering/Renderer.h"
#include "rendering/Quad.h"

#include "rendering/Tileset.h"
#include "rendering/TextureManager.h"

using namespace std;
using namespace yama;

namespace
{
vector<const char*> TextureDirectoryPerTeam =
{
    "anims1", "anims2"
};

uint32_t Indices[] = { 0, 1, 2, 2, 1, 3 };

}

class CharacterRendering
{
public:
    void supplyRenderingParts(Renderer& renderer)
    {
        m_quad.setSprite(m_tileset.getTile(0));
        m_quad.setPosition(::position(dm_this).as_vector2_t<float>().xyz(1));

        Renderer::RenderOp op;
        op.texture = m_tileset.texture();
        op.geometry = m_quad.vertices();
        op.numVertices = 4;
        op.indices = Indices;
        op.numIndices = 6;
        renderer.addRenderOp(op);
    }

    void init()
    {
        auto team = ::team(dm_this);
        string texture = TextureDirectoryPerTeam[team];
        texture += '/';
        texture += ::name(dm_this);
        texture += ".png";

        m_tileset.setTexture(TextureManager::instance().getTexture(texture));
        m_tileset.setTilesPerSide(vt(4u, 4u));
    }

private:
    Tileset m_tileset;
    Quad m_quad;
};

DYNAMIX_DEFINE_MIXIN(CharacterRendering, supplyRenderingParts_msg);

void initializeCharacterRendering(dynamix::object& object)
{
    object.get<CharacterRendering>()->init();
}