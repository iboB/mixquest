//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/States/WaitForFuturesState.h>
#include <maibo/Common/ResourceFuture.h>
#include <maibo/Rendering/GPUProgramPtr.h>

namespace Tmx
{
class Map;
}

// game state that loads all assets for the game

class LoadAllAssetsState : public maibo::WaitForFuturesState
{
    virtual bool initialize() override;
    virtual void deinitialize() override;

    virtual void beginFrame() override;
    virtual void update(uint32_t dt) override;
    virtual void render() override;
    virtual void endFrame() override;

    virtual void onDone() override;

    // stuff to add to global assets
    maibo::ResourceFuturePtr<std::shared_ptr<Tmx::Map>> m_tmxFuture;
    maibo::ResourceFuturePtr<maibo::GPUProgramPtr> m_texturizeFuture;
    maibo::ResourceFuturePtr<maibo::GPUProgramPtr> m_texturizeColorFuture;
};