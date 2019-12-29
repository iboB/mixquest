//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <memory>

// shared pointers for GPUProgram
class GPUProgram;

using GPUProgramPtr = std::shared_ptr<GPUProgram>;
using ConstGPUProgramPtr = std::shared_ptr<const GPUProgram>;
