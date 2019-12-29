//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// shared pointers for Texture

#include <memory>

class Texture;

typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<const Texture> ConstTexturePtr;
