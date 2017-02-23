//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/Common/ImplicitSingleton.h>

class Game;

class PluginManager : public maibo::ImplicitSingleton<PluginManager>
{
public:
    PluginManager();

    void setGame(Game* game);

    void loadPlugin(const std::string& name);
    void unloadPlugin(const std::string& name);

    const std::vector<std::string>& plugins() const { return m_pluginNames; }

private:

    struct Plugin
    {
    public:
        const std::string fileName;
        std::function<void(void*)> loadFunc;
        std::function<void()> unloadFunc;
        void* dynlib;
    };


    void addPlugin(const std::string& name, std::function<void(void*)> loadFunc, std::function<void()> unloadFunc);
    Plugin* findPlugin(const std::string& name);

    std::string m_pluginPath;

    // equal size
    std::vector<std::string> m_pluginNames;
    std::vector<Plugin> m_plugins;
};