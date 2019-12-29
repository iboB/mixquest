//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "InGameState.h"
#include "gameplay/game/Game.h"

#include "assets/GlobalAssets.h"
#include "map/MapRendering.h"

#include "systems/RenderingSystem.h"
#include "rendering/Renderer.h"

#include "CharacterNames.h"

#include "input/InputManager.h"

#include "mixins/KeyboardControl.mix.h"

#include "gameplay/messages/TransformMessages.h"
#include "gameplay/messages/CharacterMessages.h"

#include "plugins/PluginManager.h"

#include "gameplay/mixins/SelectedActor.mix.h"
#include "mixins/SelectionMark.mix.h"

#include "scripting/ScriptManager.h"

#include "rendering/GLSentries.h"

#include <dynamix/mutation_rule.hpp>

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace yama;
using namespace dynamix;
using namespace std;

namespace
{

class bundle : public mutation_rule, private mixin_collection
{
public:
    using mixin_collection::add;
    using mixin_collection::has;
    using mixin_collection::remove;

    virtual void apply_to(object_type_mutation& mutation, const mixin_collection&) override
    {
        // find if the mutation is adding any of the bundled mixins

        mixin_id adding = INVALID_MIXIN_ID;
        mixin_id removing = INVALID_MIXIN_ID;

        for (const auto* mixin_info : _compact_mixins)
        {
            if (mutation.is_adding(mixin_info->id))
            {
                adding = mixin_info->id;
                break;
            }
            else if (mutation.is_removing(mixin_info->id))
            {
                removing = mixin_info->id;
                break;
            }
        }

        I_DYNAMIX_ASSERT_MSG(adding == INVALID_MIXIN_ID || removing == INVALID_MIXIN_ID, "mutation breaking a bundle mixin rule");

        if (adding != INVALID_MIXIN_ID)
        {
            for (const auto* mixin_info : _compact_mixins)
            {
                // add all others to the object
                mutation.start_adding(mixin_info->id);
            }
        }
        else if(removing != INVALID_MIXIN_ID)
        {
            for (const auto* mixin_info : _compact_mixins)
            {
                // remove all others from the object
                mutation.start_removing(mixin_info->id);
            }
        }
    }
};

}

InGameState::InGameState()
    : m_camPos(v(1, 15, 0))
    , m_camTarget(m_camPos)
    , m_camMoveWeight(point2::zero())
{
}

static dynamix::object* testObj = nullptr;

bool InGameState::initialize()
{
//    auto rule = new bundle;
//    rule->add<SelectionMark>();
//    rule->add<SelectedActor>();
//    add_new_mutation_rule(rule);

    m_mapRendering = new MapRendering;

    auto& ga = GlobalAssets::instance();
    assert(ga.tmx);
    auto mapDesc = m_mapRendering->loadFromTmx(*ga.tmx);

    m_game = new Game(mapDesc);

    PluginManager::instance().setGame(m_game);

    m_renderer.reset(new Renderer);
    m_game->addSystem(new RenderingSystem(*m_renderer));

    // spawn characters at random pos
    //int teamId = 0;
    //for (auto& team : Character_Names)
    //{
    //    for (auto& name : team)
    //    {
    //        auto& c = m_game->spawnCharacter(teamId, name);
    //        mutate(c).add<KeyboardControl>();
    //        m_game->onObjectMutated(c);
    //    }
    //    ++teamId;
    //}

    ivector2 pos = vt(11, 20);
    m_camPos = m_camTarget = pos.as_vector2_t<float>().xyz(0);
    auto& c = m_game->spawnCharacter(0, "Royce", &pos);
    mutate(c).add<KeyboardControl>();
    m_game->onObjectMutated(c);

    testObj = &c;

    pos.x += 5;
    auto& e = m_game->spawnCharacter(1, "PterosMale002", &pos);
    mutate(e).add<KeyboardControl>();
    m_game->onObjectMutated(e);

    glClearColor(0.0f, 0.1f, 0.4f, 1);

    return true;
}

void InGameState::deinitialize()
{
    delete m_mapRendering;
    delete m_game;
}

bool InGameState::onKeyboardEvent(int key, int action, int mods)
{
    auto& im = InputManager::instance();

    if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_W:
        case GLFW_KEY_S:
            m_camMoveWeight.y = 0.f;
            break;
        case GLFW_KEY_A:
        case GLFW_KEY_D:
            m_camMoveWeight.x = 0.f;
            break;
        case GLFW_KEY_UP:
            im.setCommand(Command::Up);
            break;
        case GLFW_KEY_DOWN:
            im.setCommand(Command::Down);
            break;
        case GLFW_KEY_LEFT:
            im.setCommand(Command::Left);
            break;
        case GLFW_KEY_RIGHT:
            im.setCommand(Command::Right);
            break;
        case GLFW_KEY_H:
            im.setCommand(Command::SelfHarm);
            break;
        case GLFW_KEY_SPACE:
        {
            auto currentActor = m_game->currentActor();
            if (currentActor)
            {
                std::cout << "Camera moving towards: " << ::name(currentActor) << std::endl;
                m_camTarget = ::position(currentActor).as_vector2_t<float>().xyz(0);
            }
        }
            break;
        case GLFW_KEY_ENTER:
            ScriptManager::instance().testFunc(*testObj);
            return false;
        default:
            return false;
        }
    }
    else if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            m_camMoveWeight.y = 1.f;
            break;
        case GLFW_KEY_S:
            m_camMoveWeight.y = -1.f;
            break;
        case GLFW_KEY_A:
            m_camMoveWeight.x = -1.f;
            break;
        case GLFW_KEY_D:
            m_camMoveWeight.x = 1.f;
            break;
        }
    }

    return false;
}

void InGameState::beginFrame()
{

}

static bool ImGuiStringVectorItemGetter(void* data, int id, const char** outText)
{
    const auto* vec = reinterpret_cast<vector<string>*>(data);

    if (id >= int(vec->size()))
    {
        return false;
    }

    *outText = vec->at(id).c_str();
    return true;
};

void InGameState::update(uint32_t dt)
{
    auto& pm = PluginManager::instance();
    pm.update(dt);

    m_game->update(dt);

    if (m_camMoveWeight.manhattan_length() > 0)
    {
        auto n = normalize(m_camMoveWeight);
        n *= float(dt) / 100;
        m_camTarget.xy() += n;
    }

    auto dir = m_camTarget - m_camPos;
    if (dir.length_sq() < 0.0001f)
    {
        m_camPos = m_camTarget;
    }
    else
    {
        auto len = dir.length();
        dir /= len; // normalie without finding length twice
        auto mag = float(dt) / 50;
        if (len < mag)
        {
            m_camPos = m_camTarget;
        }
        else
        {
            m_camPos += dir * mag;
        }
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
        ImGui::Begin("Plugins", &showPluginsDialog, ImVec2(300, 200));

        static int curItem = 0;
        auto& plugins = pm.plugins();

        ImGui::ListBox("Files", &curItem, ImGuiStringVectorItemGetter, (void*)&plugins, int(plugins.size()), 5);

        if (ImGui::Button("Load"))
        {
            pm.loadPlugin(plugins[curItem]);
            showPluginsDialog = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Unload"))
        {
            pm.unloadPlugin(plugins[curItem]);
            showPluginsDialog = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            showPluginsDialog = false;
        }

        ImGui::End();
    }

    ImGui::End();
}

void InGameState::render()
{
    matrix proj = matrix::ortho_lh(-8, 8, -6, 6, 0, 10) * matrix::look_towards_lh(m_camPos, vector3::unit_z(), vector3::unit_y());
    glViewport(0, 0, 800, 600);

    GL_SENTRY(GLEnable, GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL_SENTRY(GLDepthWrite, false);

    m_mapRendering->render(proj);

    m_renderer->render(proj);
}

void InGameState::endFrame()
{
    InputManager::instance().setCommand(Command::None);
}

