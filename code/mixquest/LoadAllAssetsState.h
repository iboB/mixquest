//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "AppState.h"
#include "rendering/GPUProgramPtr.h"
#include <vector>

namespace Tmx
{
class Map;
}

// game state that loads all assets for the game

class LoadAllAssetsState : public AppState
{
public:
    virtual bool initialize() override;
    virtual void deinitialize() override;

    virtual void beginFrame() override;
    virtual void update(uint32_t dt) override;
    virtual void render() override;
    virtual void endFrame() override;

    // stuff to add to global assets
    std::vector<char> m_tmxData;
    std::vector<char> m_texturizeVertData;
    std::vector<char> m_texturizeFragData;
    std::vector<char> m_texturizeColorVertData;
    std::vector<char> m_texturizeColorFragData;
    std::vector<char> m_testScript;

private:
    static bool loadShader(GPUProgramPtr* program, const std::vector<char>* vert, const std::vector<char>* frag);
};