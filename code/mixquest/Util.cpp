//
// MixQuest
// Copyright (c) 2017-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Util.h"

namespace Util
{

template <typename T>
int ReadFileToVector(const char* path, std::vector<T>& outData)
{
    static_assert(sizeof(T) == 1, "o_O");

    auto f = fopen(path, "rb");

    if (!f) return 1;

    int pos = ftell(f);
    fseek(f, 0, SEEK_END);
    int fileSize = ftell(f);
    fseek(f, pos, SEEK_SET);

    outData.resize(fileSize);

    fread(outData.data(), 1, fileSize, f);

    return 0;
}

template int ReadFileToVector<char>(const char* path, std::vector<char>& outData);
template int ReadFileToVector<uint8_t>(const char* path, std::vector<uint8_t>& outData);

}