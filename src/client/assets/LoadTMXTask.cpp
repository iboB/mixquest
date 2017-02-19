//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "LoadTMXTask.h"

#include "RetainTextureTask.h"

#include "gameplay/game/MapDescription.h"

#include <Tmx.h>

#include <maibo/Rendering/TextureManager.h>
#include <maibo/Resources/ResourceManager.h>
#include <maibo/TaskManager.h>
#include <maibo/States/WaitForFuturesState.h>

using namespace maibo;

LoadTMXTask::LoadTMXTask(ConstResourceFuturePtr<std::vector<char>> readTMX, WaitForFuturesState& state)
    : future(new ResourceFuture<std::shared_ptr<Tmx::Map>>)
    , m_readTMXFuture(readTMX)
    , m_waitState(state)
{
}

bool LoadTMXTask::execute()
{
    if (!m_readTMXFuture->isDone()) return false;

    int error = m_readTMXFuture->errorCode();
    if (error != 0)
    {
        future->setProgress(1.f);
        future->setErrorCode(error);
        future->setDone();
        return true;
    }

    auto tmx = std::make_shared<Tmx::Map>();

    const auto& resourceData = m_readTMXFuture->resource();
    tmx->ParseText(resourceData.data(), resourceData.size());

    if (tmx->HasError())
    {
        future->setProgress(1.f);
        future->setErrorCode(1);
        future->setDone();
        return true;
    }

    auto& rm = ResourceManager::instance();
    auto& tm = TaskManager::instance();

    const int numTilesets = tmx->GetNumTilesets();
    for (int i = 0; i < numTilesets; ++i)
    {
        const auto tmxTileset = tmx->GetTileset(i);
        const auto& src = tmxTileset->GetImage()->GetSource();
        auto future = rm.loadTexture("assets/" + src);
        m_waitState.addFuture(future);
        auto task = new RetainTextureTask(src, future);
        tm.pushTask(task);
        m_waitState.addFuture(task->m_future);
    }

    future->resource() = tmx;
    future->setProgress(1.f);
    future->setErrorCode(0);
    future->setDone();
    return true;
}