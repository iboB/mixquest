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
