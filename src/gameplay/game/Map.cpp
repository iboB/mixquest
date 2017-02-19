//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
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
    assert(x < m_size.x && y < m_size.y);
    return m_cells[y * m_size.x + x];
}

