//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "gameplay/game/MapDescription.h"

#include <maibo/Rendering/TexturePtr.h>

namespace Tmx
{
class Map;
}

class MapRendering
{
public:
    MapRendering();
    ~MapRendering();

    // return a map description for the game
    MapDescription loadFromTmx(const Tmx::Map& tmx);

    void render(const yama::matrix& proj);

    struct Vertex
    {
        yama::point3 pos;
        yama::point2 texCoord;
        yama::vector4 color; // color is used in debug rendering only
    };

private:

    // a layer from the tmx is (potentially) split into render ops
    // one for each tileset used in the layer
    struct RenderOp
    {
        uint32_t firstIndex;
        uint32_t count;
        maibo::ConstTexturePtr texture;
    };
    std::vector<RenderOp> m_renderOps;

    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
};