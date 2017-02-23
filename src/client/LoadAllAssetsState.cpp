//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "LoadAllAssetsState.h"

#include "assets/LoadTMXTask.h"
#include "assets/GlobalAssets.h"
#include "assets/RetainTextureTask.h"

#include "InGameState.h"

#include "CharacterNames.h"

#include <maibo/Application.h>
#include <maibo/TaskManager.h>
#include <maibo/Assets/FileManager.h>
#include <maibo/Resources/ResourceManager.h>
#include <maibo/Rendering/GPUProgram.h>

bool LoadAllAssetsState::initialize()
{
    //auto& rm = maibo::ResourceManager::instance();

    auto& fm = maibo::FileManager::instance();

    auto mapFileFuture = fm.readFileAsync("assets/map.tmx", true);
    addFuture(mapFileFuture);

    auto task = new LoadTMXTask(mapFileFuture, *this);
    auto& tm = maibo::TaskManager::instance();
    tm.pushTask(task);
    addFuture(task->future);

    m_tmxFuture = task->future;

    auto& rm = maibo::ResourceManager::instance();
    m_texturizeFuture = rm.loadGPUProgramAsync("assets/texturize.vert", "assets/texturize.frag", true);
    addFuture(m_texturizeFuture);

    m_texturizeColorFuture = rm.loadGPUProgramAsync("assets/tex_color.vert", "assets/tex_color.frag", true);
    addFuture(m_texturizeColorFuture);

    char animsNum[2] = "0";
    for (int i = 0; i < 2; ++i)
    {
        ++animsNum[0];

        for (auto& name : Character_Names[i])
        {
            auto textureName = std::string("anims") + animsNum + "/" + name + ".png";
            auto future = rm.loadTexture("assets/" + textureName);
            addFuture(future);
            auto task = new RetainTextureTask(textureName, future);
            tm.pushTask(task);
            addFuture(task->m_future);
        }
    }

    auto future = rm.loadTexture("assets/health.png");
    addFuture(future);
    auto rttask = new RetainTextureTask("health.png", future);
    tm.pushTask(rttask);
    addFuture(rttask->m_future);

    future = rm.loadTexture("assets/selection.png");
    addFuture(future);
    rttask = new RetainTextureTask("selection.png", future);
    tm.pushTask(rttask);
    addFuture(rttask->m_future);

    glClearColor(1, 0.1f, 0.4f, 1);

    return true;
}

void LoadAllAssetsState::deinitialize()
{
}

void LoadAllAssetsState::beginFrame()
{
}

void LoadAllAssetsState::update(uint32_t dt)
{
    maibo::WaitForFuturesState::update(dt);
}

void LoadAllAssetsState::render()
{
}

void LoadAllAssetsState::endFrame()
{
}

void LoadAllAssetsState::onDone()
{
    auto& ga = GlobalAssets::instance();
    ga.tmx = m_tmxFuture->resource();

    ga.texturizeMaterial.m_program = m_texturizeFuture->resource();
    ga.texturizeMaterial.init();

    ga.texturizeColorMaterial.m_program = m_texturizeColorFuture->resource();
    ga.texturizeColorMaterial.init();

    maibo::Application_Instance().setState(new InGameState);
}

