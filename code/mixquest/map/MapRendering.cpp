//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "MapRendering.h"

#include "assets/GlobalAssets.h"
#include "Memory.h"

MapRendering::MapRendering()
{
    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_indexBuffer);
}

MapRendering::~MapRendering()
{
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_indexBuffer);
}

void MapRendering::render(const yama::matrix& proj)
{
    auto& material = GlobalAssets::instance().texturizeMaterial;

    material.prepareBuffer(m_vertexBuffer, sizeof(Vertex), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    material.begin();

    material.setProjection(proj);

    for (auto& op : m_renderOps)
    {
        material.setTexture(*op.texture);
        glDrawElements(GL_TRIANGLES, op.count, GL_UNSIGNED_INT, OFFSET(op.firstIndex * sizeof(uint32_t)));
    }

    material.end();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
