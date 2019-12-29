//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "ImplicitSingleton.h"

class Game;
class HotswapFSWatcher;

class PluginManager : public ImplicitSingleton<PluginManager>
{
public:
    PluginManager();
    ~PluginManager();

    // call to use fs watcher
    void update(uint32_t dt);

    void setGame(Game* game);

    void loadPlugin(const std::string& name);
    void unloadPlugin(const std::string& name);

    const std::vector<std::string>& plugins() const { return m_pluginNames; }

    const std::string& buildOutputDir() const { return m_buildOutputDir; }
    const std::string& pluginsDir() const { return m_pluginsDir; }

    // typically called by filesystem watcher
    void onFileChanged(const std::string& filename);

private:
    struct Plugin
    {
    public:
        const std::string filename;
        std::function<void(void*)> loadFunc;
        std::function<void()> unloadFunc;
        void* dynlib;
    };


    void addPlugin(const std::string& name, std::function<void(void*)> loadFunc, std::function<void()> unloadFunc);
    Plugin* findPlugin(const std::string& name);

    void loadPlugin(Plugin& p);
    void reloadPlugin(Plugin& p);
    void unloadPlugin(Plugin& p);

    std::string m_buildOutputDir; // where to watch for changes
    std::string m_pluginsDir; // where to copy plugins

    // used to prependd a unique identifier to the plugins so different executables
    // (or different instances of the same executable) can use the same directory and same plugins
    const intptr_t m_uid;

    // equal size
    std::vector<std::string> m_pluginNames;
    std::vector<Plugin> m_plugins;

    std::unique_ptr<HotswapFSWatcher> m_watcher;
};