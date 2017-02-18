//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "MixQuest.h"

#include <maibo/Resources/ResourceManager.h>
#include <maibo/Rendering/TextureManager.h>
#include <maibo/GUI/ImGui/ImGuiManager.h>

#include "LoadAllAssetsState.h"
#include "assets/GlobalAssets.h"

using namespace std;
using namespace maibo;

bool MixQuestApp::initialize()
{
    Application::CreationParameters params;

    params.mainWindowTitle = "MixQuest";
    params.desiredFrameTimeMs = 10;

    if (!Application::initialize(params))
    {
        return false;
    }

    TextureManager::createInstance();
    GlobalAssets::createInstance();
    ImGuiManager::createInstance();

    // You can configure opengl here
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    setState(new LoadAllAssetsState);

    startRunning();

    return true;
}

void MixQuestApp::deinitialize()
{
    ImGuiManager::destroyInstance();
    TextureManager::destroyInstance();
    GlobalAssets::destroyInstance();
    Application::deinitialize();

    cout << "Total frames: " << totalFrames() << endl;
}

void MixQuestApp::update()
{
    Application::update();
}

void MixQuestApp::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Application::render();
}

void MixQuestApp::onSetStateError(AppState* state)
{
    // nothing to do yet
}

// maibo integration
namespace maibo
{
    namespace
    {
        MixQuestApp* app = nullptr;
    }

    void Application_CreateInstance(int argc, char* argv[])
    {
        app = new MixQuestApp();
    }

    Application& Application_Instance()
    {
        return *app;
    }

    void Application_DestroyInstance(int retCode)
    {
        delete app;
    }
}
