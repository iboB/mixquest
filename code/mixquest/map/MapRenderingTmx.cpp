//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "MapRendering.h"

#include <Tmx.h>

#include "rendering/Tileset.h"
#include "rendering/TextureManager.h"
#include "Memory.h"

#include <array>

using namespace std;
using namespace yama;

namespace
{

struct Quad
{
    MapRendering::Vertex v00, v10, v01, v11;
};


class QuadIndexGenerator
{
public:
    QuadIndexGenerator()
        : m_wrap(0)
        , m_indicesArray({ 0, 1, 2, 2, 1, 3 })
    {}

    uint32_t operator ()()
    {
        if (m_wrap == m_indicesArray.size())
        {
            m_wrap = 0;

            // shift to next tile
            for (auto& index : m_indicesArray)
            {
                index += 4;
            }
        }
        return m_indicesArray[m_wrap++];
    }

private:
    size_t m_wrap;
    std::array<uint32_t, 6> m_indicesArray;
};

}

MapDescription MapRendering::loadFromTmx(const Tmx::Map& tmx)
{
    const int numTilesets = tmx.GetNumTilesets();
    vector<Tileset> tilesets;
    tilesets.resize(numTilesets);
    auto& tm = TextureManager::instance();
    for (int i = 0; i < numTilesets; ++i)
    {
        auto& tileset = tilesets[i];

        const auto tmxTileset = tmx.GetTileset(i);

        tileset.setTexture(tm.getTexture(tmxTileset->GetImage()->GetSource()));
        tileset.setSpacing(tmxTileset->GetSpacing());
        tileset.setMargin(tmxTileset->GetMargin());
        tileset.setTileSize(vt(uint32_t(tmxTileset->GetTileWidth()), uint32_t(tmxTileset->GetTileHeight())));
    }

    MapDescription map;
    map.size = vt(tmx.GetWidth(), tmx.GetHeight());
    map.tiles.resize(map.size.product());

    vector<vector<vector<Quad>>> renderOpData; // per layer per tileset
    renderOpData.resize(tmx.GetNumTileLayers());

    uint32_t numQuads = 0;
    for (int i = 0; i < tmx.GetNumTileLayers(); ++i)
    {
        renderOpData[i].resize(numTilesets);

        const auto layer = tmx.GetTileLayer(i);

        for (int y = 0; y < layer->GetHeight(); ++y)
        {
            // since logical world coordinates are 00 -> bottom left
            // and tile coordinates are 00 -> top left
            // do a conversion
            float top = float(map.size.y - y);
            float bottom = top - 1;

            for (int x = 0; x < layer->GetWidth(); ++x)
            {
                const auto& tile = layer->GetTile(x, y);
                if (tile.tilesetId == -1)
                {
                    continue;
                }

                auto sprite = tilesets[tile.tilesetId].getTile(tile.id);

                if (tile.flippedHorizontally)
                    sprite.tc.flipHorizontally();

                if (tile.flippedVertically)
                    sprite.tc.flipVertically();

                if (tile.flippedDiagonally)
                    sprite.tc.flipVertically();

                auto& data = renderOpData[i][tile.tilesetId];
                data.resize(data.size() + 1);
                auto& quad = data.back();

                // generate tile quad
                float left = float(x);
                float right = left + 1;

                quad.v00.pos = v(left, bottom, 0);
                quad.v10.pos = v(right, bottom, 0);
                quad.v01.pos = v(left, top, 0);
                quad.v11.pos = v(right, top, 0);

                quad.v00.texCoord = sprite.tc._01;
                quad.v10.texCoord = sprite.tc._11;
                quad.v01.texCoord = sprite.tc._00;
                quad.v11.texCoord = sprite.tc._10;

                auto tmxTileset = tmx.GetTileset(tile.tilesetId);
                auto tmxTile = tmxTileset->GetTile(tile.id);

                vector4 color;
                // invert here too, to match coordinates
                const int tilesIndex = (map.size.y - y - 1) * map.size.x + x;
                if (tmxTile && tmxTile->GetProperties().GetBoolProperty("walk"))
                {
                    color = v(0, 1, 0, 1);
                    map.tiles[tilesIndex] = false;
                }
                else
                {
                    color = v(1, 0, 0, 1);
                    map.tiles[tilesIndex] = true;
                }

                quad.v00.color = color;
                quad.v10.color = color;
                quad.v01.color = color;
                quad.v11.color = color;

                ++numQuads;
            }
        }
    }

    // create buffers
    const auto bufferSize = numQuads * 4 * sizeof(Vertex);
    char* const bufferData = new char[bufferSize];
    char* bufferPtr = bufferData;

    uint32_t indexOffset = 0;
    m_renderOps.reserve(tmx.GetNumTileLayers() * numTilesets);
    for (auto& layer : renderOpData)
    {
        for(size_t i=0; i<layer.size(); ++i)
        {
            auto& quads = layer[i];
            if (quads.empty()) continue;

            auto& tileset = tilesets[i];

            m_renderOps.resize(m_renderOps.size() + 1);
            auto& op = m_renderOps.back();
            op.firstIndex = indexOffset;
            op.count = uint32_t(quads.size()) * 6;
            op.texture = tileset.texture();

            auto size = quads.size() * 4 * sizeof(Vertex);
            memcpy(bufferPtr, quads.data(), size);

            bufferPtr += size;
            indexOffset += op.count;
        }
    }

    assert(bufferPtr - bufferData == bufferSize);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
    delete[] bufferData;

    // indices
    vector<uint32_t> indices;
    indices.resize(6 * numQuads);
    QuadIndexGenerator gen;
    generate(indices.begin(), indices.end(), gen);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size(indices), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return map;
}
