//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "Vertex.h"

#include <maibo/Rendering/TexturePtr.h>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    struct RenderOp
    {
        const Vertex* geometry = nullptr;
        int numVertices = 0;
        const uint32_t* indices = nullptr;
        int numIndices = 0;
        maibo::ConstTexturePtr texture;
    };

    // call after beginFrame and before render
    void addRenderOp(const RenderOp& op);

    void render(yama::matrix& camera);

private:
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;

    // frame data
    std::vector<Vertex> m_frameVertices;
    std::vector<uint32_t> m_frameIndices;
    std::vector<RenderOp> m_frameRenderOps;
};
