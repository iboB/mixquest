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

#include <maibo/States/AppState.h>

class Game;
class MapRendering;

class InGameState : public maibo::AppState
{
public:
    InGameState();

    virtual bool initialize() override;
    virtual void deinitialize() override;

    virtual bool handleEvent(const SDL_Event& event) override;

    virtual void beginFrame() override;
    virtual void update(uint32_t dt) override;
    virtual void render() override;
    virtual void endFrame() override;

private:
    Game* m_game = nullptr;
    MapRendering* m_mapRendering = nullptr;
    yama::point3 m_camPos;
    yama::point2 m_camMoveWeight;
};
