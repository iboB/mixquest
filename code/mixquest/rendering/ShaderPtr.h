//
// MixQuest
// Copyright (c) 2017-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <memory>

// shared pointers for Shader
class Shader;

using ShaderPtr = std::shared_ptr<Shader>;
using ConstShaderPtr = std::shared_ptr<const Shader>;
