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

#include "InGameState.h"

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
    maibo::TaskManager::instance().pushTask(task);
    addFuture(task->future);

    m_tmxFuture = task->future;

    auto& rm = maibo::ResourceManager::instance();
    m_texturizeFuture = rm.loadGPUProgramAsync("assets/texturize.vert", "assets/texturize.frag", true);
    addFuture(m_texturizeFuture);

    m_texturizeColorFuture = rm.loadGPUProgramAsync("assets/tex_color.vert", "assets/tex_color.frag", true);
    addFuture(m_texturizeColorFuture);

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

