//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <cstddef>

#define OFFSET(i) ((char *)nullptr + (i))
#define OFFSET_OF(type, member) (&static_cast<type*>(nullptr)->member)

template <typename Container>
size_t data_size(const Container& vec)
{
    return vec.size() * sizeof(typename Container::value_type);
}
