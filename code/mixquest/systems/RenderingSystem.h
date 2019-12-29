//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "gameplay/systems/GameplaySystem.h"

class Renderer;

class RenderingSystem : public GameplaySystem
{
public:
    RenderingSystem(Renderer& renderer);

    virtual void update(uint32_t dms) override;
    virtual bool checkObject(dynamix::object& object) const override;

private:
    Renderer& m_renderer;
};
