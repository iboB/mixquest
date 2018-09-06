//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// Main state for playing the game

#include "AppState.h"

class Game;
class MapRendering;
class Renderer;

class InGameState : public AppState
{
public:
    InGameState();

    virtual bool initialize() override;
    virtual void deinitialize() override;

    virtual bool onKeyboardEvent(int key, int action, int mods) override;

    virtual void beginFrame() override;
    virtual void update(uint32_t dt) override;
    virtual void render() override;
    virtual void endFrame() override;

private:
    Game* m_game = nullptr;
    MapRendering* m_mapRendering = nullptr;
    yama::point3 m_camPos;
    yama::point3 m_camTarget;
    yama::point2 m_camMoveWeight;

    std::unique_ptr<Renderer> m_renderer;
};
