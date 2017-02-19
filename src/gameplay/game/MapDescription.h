//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

struct GAMEPLAY_API MapDescription
{
    yama::ivector2 size;

    // true when a tile is accessible
    std::vector<bool> tiles;

    struct Object
    {
        yama::ivector2 pos;
        int type;
    };
    std::vector<Object> objects;
};
