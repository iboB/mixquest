//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Map.h"
#include "MapDescription.h"

Map::Map(const MapDescription& desc)
    : m_size(desc.size)
{
    m_cells.resize(desc.tiles.size());

    for (size_t i = 0; i < m_cells.size(); ++i)
    {
        m_cells[i].walkable = !desc.tiles[i];
    }
}

const Map::Cell& Map::cell(int x, int y) const
{
    static Cell outside;
    if (x < 0 || x >= m_size.x || y < 0 || y >= m_size.y)
        return outside;

    return m_cells[y * m_size.x + x];
}

Map::Cell* Map::pcell(yama::ivector2 coord)
{
    if (coord.x < 0 || coord.x >= m_size.x || coord.y < 0 || coord.y >= m_size.y)
        return nullptr;

    return &m_cells[coord.y * m_size.x + coord.x];
}

void Map::addObject(dynamix::object& obj, yama::ivector2 coord)
{
    auto c = pcell(coord);
    assert(c);
    c->objects.push_back(&obj);
}

void Map::moveObject(dynamix::object& obj, yama::ivector2 from, yama::ivector2 to)
{
    auto cf = pcell(from);
    assert(cf);
    auto ct = pcell(to);
    assert(ct);

    auto f = find(cf->objects.begin(), cf->objects.end(), &obj);
    assert(f != cf->objects.end());
    cf->objects.erase(f);

    ct->objects.push_back(&obj);
}
