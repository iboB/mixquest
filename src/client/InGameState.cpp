//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "InGameState.h"
#include "gameplay/game/Game.h"

#include "assets/GlobalAssets.h"
#include "map/MapRendering.h"

#include <maibo/Common/GLSentries.h>

#include <imgui.h>

using namespace yama;
using namespace dynamix;
using namespace maibo;

InGameState::InGameState()
    : m_camPos(v(1, 15, 0))
    , m_camMoveWeight(point2::zero())
{
}

bool InGameState::initialize()
{
    m_mapRendering = new MapRendering;

    auto& ga = GlobalAssets::instance();
    assert(ga.tmx);
    auto mapDesc = m_mapRendering->loadFromTmx(*ga.tmx);

    m_game = new Game(mapDesc);

    glClearColor(0.0f, 0.1f, 0.4f, 1);

    return true;
}

void InGameState::deinitialize()
{
    delete m_mapRendering;
    delete m_game;
}

bool InGameState::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
        case SDLK_s:
            m_camMoveWeight.y = 0.f;
            break;
        case SDLK_a:
        case SDLK_d:
            m_camMoveWeight.x = 0.f;
            break;
        case SDLK_UP:
            break;
        case SDLK_DOWN:
            break;
        case SDLK_LEFT:
            break;
        case SDLK_RIGHT:
            break;
        case SDLK_RETURN:
        case SDLK_RETURN2:
            return false;
        default:
            return false;
        }
    }
    else if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            m_camMoveWeight.y = 1.f;
            break;
        case SDLK_s:
            m_camMoveWeight.y = -1.f;
            break;
        case SDLK_a:
            m_camMoveWeight.x = -1.f;
            break;
        case SDLK_d:
            m_camMoveWeight.x = 1.f;
            break;
        }
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
        }
    }

    return false;
}

void InGameState::beginFrame()
{

}


void InGameState::update(uint32_t dt)
{
    m_game->update(dt);

    if (m_camMoveWeight.manhattan_length() > 0)
    {
        auto n = normalize(m_camMoveWeight);
        n *= float(dt) / 100;
        m_camPos.xy() += n;
    }

    static bool showImGuiHud = true;
    ImGui::SetNextWindowPos(ImVec2());
    ImGui::Begin("Hud", &showImGuiHud, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
    
    static bool showPluginsDialog = false;
    if (ImGui::Button("Plugins..."))
    {
        showPluginsDialog = true;
    }

    if (showPluginsDialog)
    {
        ImGui::Begin("Plugins", &showPluginsDialog);

        static const char* files[] = { "AI", "Test1", "Test2" };
        static int curItem = 0;
        ImGui::ListBox("Files", &curItem, files, sizeof(files)/sizeof(char*), 10);

        ImGui::Columns(3);

        if (ImGui::Button("Load"))
        {
            std::cout << "Loading " << files[curItem] << "\n";
            showPluginsDialog = false;
        }

        ImGui::NextColumn();

        if (ImGui::Button("Unload"))
        {
            std::cout << "Unloading " << files[curItem] << "\n";
            showPluginsDialog = false;
        }

        ImGui::NextColumn();

        if (ImGui::Button("Cancel"))
        {
            showPluginsDialog = false;
        }

        ImGui::Columns(1);

        ImGui::End();
    }
    
    ImGui::End();
}

void InGameState::render()
{
    matrix proj = matrix::ortho_lh(-8, 8, -6, 6, 0, 10) * matrix::look_towards_lh(m_camPos, vector3::unit_z(), vector3::unit_y());

    MAIBO_GL_SENTRY(GLEnable, GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    MAIBO_GL_SENTRY(GLDepthWrite, false);

    m_mapRendering->render(proj);
}

void InGameState::endFrame()
{
}

