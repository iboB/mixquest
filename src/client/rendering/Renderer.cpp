//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Renderer.h"

#include "assets/GlobalAssets.h"
#include "assets/TexturizeMaterial.h"

using namespace maibo;

Renderer::Renderer()
{
    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_indexBuffer);
}

Renderer::~Renderer()
{
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_indexBuffer);
}

void Renderer::addRenderOp(const RenderOp& op)
{
    m_frameRenderOps.push_back(op);
    uint32_t base = uint32_t(m_frameVertices.size());
    m_frameVertices.insert(m_frameVertices.end(), op.geometry, op.geometry + op.numVertices);
    m_frameIndices.reserve(m_frameIndices.size() + op.numIndices);
    for (int i = 0; i < op.numIndices; ++i)
    {
        m_frameIndices.push_back(base + op.indices[i]);
    }
}

void Renderer::render(yama::matrix& camera)
{
    // fill buffers
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, data_size(m_frameVertices), m_frameVertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size(m_frameIndices), m_frameIndices.data(), GL_DYNAMIC_DRAW);

    auto& material = GlobalAssets::instance().texturizeMaterial;

    material.prepareBuffer(m_vertexBuffer, sizeof(Vertex), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    material.begin();

    material.setProjection(camera);

    uint32_t offset = 0;
    for (auto& op : m_frameRenderOps)
    {
        material.setTexture(*op.texture);
        glDrawElements(GL_TRIANGLES, op.numIndices, GL_UNSIGNED_INT, OFFSET(offset * sizeof(uint32_t)));
        offset += op.numIndices;
    }

    material.end();

    m_frameVertices.clear();
    m_frameIndices.clear();
    m_frameRenderOps.clear();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

