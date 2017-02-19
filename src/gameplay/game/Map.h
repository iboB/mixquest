//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

struct MapDescription;

class Map
{
public:
    Map(const MapDescription& desc);

    struct Cell
    {
        bool walkable;
        std::vector<dynamix::object*> objects;
    };

    const Cell& cell(int x, int y) const;
    const Cell& cell(yama::ivector2 coord) const {
        return cell(coord.x, coord.y);
    }

    const yama::ivector2& size() const {
        return m_size;
    }

private:
    const yama::ivector2 m_size;
    std::vector<Cell> m_cells;
};
