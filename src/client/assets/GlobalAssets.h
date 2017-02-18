//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/Common/ExplicitSingleton.h>

#include "assets/TexturizeMaterial.h"
#include "assets/TexturizeColorMaterial.h"

namespace Tmx
{
class Map;
}

// just a simple global registry of all assets
struct GlobalAssets : public maibo::ExplicitSingleton<GlobalAssets>
{
    MAIBO_DECLARE_EXPLICIT_SINGLETON(GlobalAssets);
public:
    std::shared_ptr<Tmx::Map> tmx;
    TexturizeMaterial texturizeMaterial;
    TexturizeColorMaterial texturizeColorMaterial;
};