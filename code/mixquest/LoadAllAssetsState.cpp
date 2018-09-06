//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "LoadAllAssetsState.h"
#include "assets/GlobalAssets.h"
#include "scripting/ScriptManager.h"
#include "InGameState.h"
#include "CharacterNames.h"
#include "Application.h"
#include "Util.h"
#include "assets/LoadTMX.h"
#include "rendering/Shader.h"
#include "rendering/TextureManager.h"
#include "rendering/Texture.h"
#include "rendering/GPUProgram.h"

using namespace std;

bool LoadAllAssetsState::initialize()
{
    auto& app = Application::instance();

    app.pushTask([this]() {
        Util::ReadFileToVector("assets/map.tmx", m_tmxData);
        return true;
    });

    app.pushTask(std::bind(Util::LoadTMX, this));

    app.pushTask([this]() {
        Util::ReadFileToVector("assets/texturize.vert", m_texturizeVertData);
        m_texturizeVertData.push_back(0);
        return true;
    });

    app.pushTask([this]() {
        Util::ReadFileToVector("assets/texturize.frag", m_texturizeFragData);
        m_texturizeFragData.push_back(0);
        return true;
    });

    auto& a = GlobalAssets::instance();
    app.pushTask(std::bind(&LoadAllAssetsState::loadShader, &a.texturizeMaterial.m_program, &m_texturizeVertData, &m_texturizeFragData));

    app.pushTask([&a]() {
        a.texturizeMaterial.init();
        return true;
    });
    
    app.pushTask([this]() {
        Util::ReadFileToVector("assets/tex_color.vert", m_texturizeColorVertData);
        m_texturizeColorVertData.push_back(0);
        return true;
    });

    app.pushTask([this]() {
        Util::ReadFileToVector("assets/tex_color.frag", m_texturizeColorFragData);
        m_texturizeColorFragData.push_back(0);
        return true;
    });

    app.pushTask(std::bind(&LoadAllAssetsState::loadShader, &a.texturizeColorMaterial.m_program, &m_texturizeColorVertData, &m_texturizeColorFragData));

    app.pushTask([&a]() {
        a.texturizeColorMaterial.init();
        return true;
    });

    auto& tm = TextureManager::instance();
    char animsNum[2] = "0";
    for (int i = 0; i < 2; ++i)
    {
        ++animsNum[0];

        for (auto& name : Character_Names[i])
        {
            auto textureName = std::string("anims") + animsNum + "/" + name + ".png";
            string path = "assets/" + textureName;

            app.pushTask([path, textureName, &tm] {
                auto tex = make_shared<Texture>(textureName);
                tex->loadFromFile(path.c_str());
                tm.addTexture(tex, textureName, true);
                return true;
            });
        }
    }
    app.pushTask([&tm] {
        auto tex = make_shared<Texture>("health.png");
        tex->loadFromFile("assets/health.png");
        tm.addTexture(tex, "health.png", true);
        return true;
    });

    app.pushTask([&tm] {
        auto tex = make_shared<Texture>("selection.png");
        tex->loadFromFile("assets/selection.png");
        tm.addTexture(tex, "selection.png", true);
        return true;
    });

    app.pushTask([this]() {
        Util::ReadFileToVector("assets/test.lua", m_testScript);
        return true;
    });

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
    auto& app = Application::instance();

    if (app.hasPendingTasks()) return;

    auto& ga = GlobalAssets::instance();

    ga.texturizeMaterial.init();
    ga.texturizeColorMaterial.init();
    ScriptManager::instance().loadScript(m_testScript);

    app.setState(new InGameState);
}

void LoadAllAssetsState::render()
{
}

void LoadAllAssetsState::endFrame()
{
}

bool LoadAllAssetsState::loadShader(GPUProgramPtr* program, const std::vector<char>* vert, const std::vector<char>* frag)
{
    auto vs = make_shared<Shader>(ShaderType::Vertex);
    vs->load(vert->data());

    auto fs = make_shared<Shader>(ShaderType::Fragment);
    fs->load(frag->data());

    auto& p = *program;
    p = make_shared<GPUProgram>();
    p->attachShader(vs);
    p->attachShader(fs);
    p->link();

    return true;
}

