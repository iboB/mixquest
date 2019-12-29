//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "ExplicitSingleton.h"

#include "assets/TexturizeMaterial.h"
#include "assets/TexturizeColorMaterial.h"

#include <memory>

namespace Tmx
{
class Map;
}

// just a simple global registry of all assets
struct GlobalAssets : public ExplicitSingleton<GlobalAssets>
{
    DECLARE_EXPLICIT_SINGLETON(GlobalAssets);
public:
    std::shared_ptr<Tmx::Map> tmx;
    TexturizeMaterial texturizeMaterial;
    TexturizeColorMaterial texturizeColorMaterial;
};
