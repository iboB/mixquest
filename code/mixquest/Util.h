//
// MixQuest
// Copyright (c) 2017-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <cstdint>
#include <vector>

namespace Util
{

template <typename T>
int ReadFileToVector(const char* path, std::vector<T>& outData);

}
