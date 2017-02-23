//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "PluginManager.h"

#include <maibo/Application.h>

#define PLUGIN_DEF
#include "plugins/ai/AI.plugin.h"
#undef PLUGIN_DEF

PluginManager::Plugin* PluginManager::findPlugin(const std::string& name)
{
    assert(m_pluginNames.size() == m_plugins.size());

    auto f = std::find(m_pluginNames.begin(), m_pluginNames.end(), name);

    if (f == m_pluginNames.end())
        return nullptr;

    return &m_plugins[f - m_pluginNames.begin()];
}

#if defined NO_PLUGINS

using namespace std;

PluginManager::PluginManager()
{
}

void PluginManager::setGame(Game* game)
{
    addPlugin("ai",
    [game](void* dl) {
        AIPluginLoad(game);
    },
    []() {
        AIPluignUnload();
    });
}

void PluginManager::addPlugin(const std::string& name, std::function<void(void*)> loadFunc, std::function<void()> unloadFunc)
{
    Plugin p = { string(), loadFunc, unloadFunc, nullptr };
    m_pluginNames.emplace_back(name);
    m_plugins.emplace_back(move(p));
}

void PluginManager::loadPlugin(const std::string& name)
{
    auto p = findPlugin(name);
    if (!p || p->dynlib) return;
    p->dynlib = reinterpret_cast<void*>(1);
    p->loadFunc(p->dynlib);
}

void PluginManager::unloadPlugin(const std::string& name)
{
    auto p = findPlugin(name);
    if (!p || !p->dynlib) return;
    p->unloadFunc();
    p->dynlib = nullptr;
}

#else

#if defined (WIN32)

#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>

typedef HMODULE DynamicLib;

#   define LoadDynamicLib(lib) LoadLibrary(lib)
#   define UnloadDynamicLib FreeLibrary
#   define GetProc GetProcAddress

#else

#   include <dlfcn.h>

typedef void* DynamicLib;
#   define LoadDynamicLib(lib) dlopen(lib, RTLD_NOW)
#   define UnloadDynamicLib dlclose
#   define GetProc dlsym

#endif

using namespace std;

PluginManager::PluginManager()
{
    const auto& exe = maibo::Application_Instance().fileName();
    m_pluginPath.assign(exe.begin(), exe.begin() + exe.rfind('/'));
}

void PluginManager::setGame(Game* game)
{
    addPlugin("ai",
    [game](void* dl) {
        auto dynlib = reinterpret_cast<DynamicLib>(dl);
#define PLUGIN_LOAD
#include "plugins/ai/AI.plugin.h"
#undef PLUGIN_LOAD
        AIPluginLoad(game);
    },
    []() {
        AIPluignUnload();
    });
}

void PluginManager::addPlugin(const std::string& name, std::function<void(void*)> loadFunc, std::function<void()> unloadFunc)
{
#if defined(_WIN32)
    auto filename = name + ".dll";
#elif defined(__APPLE__)
    auto filename = "lib" + name + ".dylib";
#else
    auto filename = "lib" + name + ".so";
#endif

    auto path = m_pluginPath + '/' + move(filename);

    Plugin p = { move(path), loadFunc, unloadFunc, false };
    m_pluginNames.emplace_back(name);
    m_plugins.emplace_back(move(p));
}

void PluginManager::loadPlugin(const std::string& name)
{
    auto p = findPlugin(name);
    if (!p || p->dynlib) return;
    p->dynlib = LoadDynamicLib(p->fileName.c_str());
    p->loadFunc(p->dynlib);
}

void PluginManager::unloadPlugin(const std::string& name)
{
    auto p = findPlugin(name);
    if (!p || !p->dynlib) return;
    p->unloadFunc();
    UnloadDynamicLib(reinterpret_cast<DynamicLib>(p->dynlib));
    p->dynlib = nullptr;
}

#endif