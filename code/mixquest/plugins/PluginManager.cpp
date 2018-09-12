//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "PluginManager.h"

#include "HotswapFSWatcher.h"

#include "Application.h"

#include <ctime>
#include <sys/stat.h>
#include <fstream>
#include <iostream>

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

namespace
{
bool exists(const std::string& filename)
{
    FILE* pFile = fopen(filename.c_str(), "rb");

    if (pFile)
    {
        fclose(pFile);

        return true;
    }

    return false;
}

void copy_if_newer(const std::string& fsrc, const std::string& fdst)
{
    if (!exists(fsrc))
    {
        remove(fdst.c_str());
        return;
    }

    if (exists(fdst))
    {
        struct stat ssrc, sdst;
        stat(fsrc.c_str(), &ssrc);
        stat(fdst.c_str(), &sdst);

        auto diff = std::difftime(ssrc.st_mtime, sdst.st_mtime);
        if (diff < 0)
        {
            return;
        }
    }

    std::ifstream src(fsrc.c_str(), std::ios::binary);
    std::ofstream dst(fdst.c_str(), std::ios::binary);
    dst << src.rdbuf();
}
}

using namespace std;

PluginManager::PluginManager()
    : m_uid(0)
{
    const auto& exe = Application::instance().fileName();
    m_buildOutputDir.assign(exe.begin(), exe.begin() + exe.rfind('/'));
    m_pluginsDir = m_buildOutputDir + "/plugins";

#if defined(_WIN32)
    system(("mkdir " + m_pluginsDir).c_str());
#else
    system(("mkdir -p " + m_pluginsDir).c_str());
#endif

    m_watcher.reset(new HotswapFSWatcher(*this));
}

PluginManager::~PluginManager() = default;

void PluginManager::update(uint32_t dt)
{
    m_watcher->update(dt);
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

    Plugin p = { move(filename), loadFunc, unloadFunc, nullptr };
    m_pluginNames.emplace_back(name);
    m_plugins.emplace_back(move(p));
}

void PluginManager::onFileChanged(const std::string& filename)
{
    for (auto& p : m_plugins)
    {
        if (p.filename == filename && p.dynlib) // only reload loaded plugins
        {
            reloadPlugin(p);
            return;
        }
    }
}

void PluginManager::loadPlugin(const std::string& name)
{
    auto p = findPlugin(name);
    if (!p || p->dynlib) return;
    loadPlugin(*p);
}

void PluginManager::unloadPlugin(const std::string& name)
{
    auto p = findPlugin(name);
    if (!p || !p->dynlib) return;
    unloadPlugin(*p);
}

void PluginManager::loadPlugin(Plugin& p)
{
    assert(!p.dynlib);

    string uid = (m_uid > 0) ? to_string(m_uid) : string();

    std::string pluginPath = m_pluginsDir + "/" + uid + p.filename;

    std::string src = m_buildOutputDir + "/" + p.filename;
    copy_if_newer(src, pluginPath);

    p.dynlib = LoadDynamicLib(pluginPath.c_str());

    if (p.dynlib)
    {
        p.loadFunc(p.dynlib);
    }
}

void PluginManager::unloadPlugin(Plugin& p)
{
    if (p.dynlib)
    {
        p.unloadFunc();
        UnloadDynamicLib(reinterpret_cast<DynamicLib>(p.dynlib));
        p.dynlib = nullptr;
    }
}

void PluginManager::reloadPlugin(Plugin& p)
{
    std::cout << "Hotswapping " << p.filename << endl;
    unloadPlugin(p);
    loadPlugin(p);
}

#endif
