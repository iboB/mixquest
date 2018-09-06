//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "LoadTMX.h"
#include "GlobalAssets.h"
#include "LoadAllAssetsState.h"
#include "gameplay/game/MapDescription.h"
#include "rendering/TextureManager.h"
#include <Tmx.h>
#include <iostream>

using namespace std;

namespace Util
{
bool LoadTMX(const LoadAllAssetsState* state)
{
    auto& a = GlobalAssets::instance();
    a.tmx = make_shared<Tmx::Map>();
    a.tmx->ParseText(state->m_tmxData.data(), state->m_tmxData.size());
    if (a.tmx->HasError())
    {
        cerr << "Could not load tmx\n";
        return true;
    }

    auto& tm = TextureManager::instance();
    const int numTilesets = a.tmx->GetNumTilesets();
    for (int i = 0; i < numTilesets; ++i)
    {
        const auto tmxTileset = a.tmx->GetTileset(i);
        const auto& src = tmxTileset->GetImage()->GetSource();
        auto tex = tm.loadTexture("assets/" + src, true);
        tm.addTexture(tex, src, true);
    }

    return true;
}
}
