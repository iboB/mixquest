//
// MixQuest
// Copyright (c) 2017-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <string>
#include <unordered_map>

namespace FW
{
class FileWatcher;
}

class PluginManager;

class HotswapFSWatcher
{
public:
    // watches for dynamic libraries which appear in buildOutput of manager
    // and invokes a hotswap on update
    HotswapFSWatcher(PluginManager& manager);
    ~HotswapFSWatcher();

    void addFileToHotswap(const std::string& file);

    // Call in a place where it's safe to hotswap shared libraries
    // the actual hotswap happens here
    void update(uint32_t dt);

private:
    PluginManager& m_pluginManager;

    // Maps a filename to a notification timer in ms.
    // To avoid hotswapping incomplete files we only activate the actual swap
    // 100 ms after the last update for a file
    std::unordered_map<std::string, int> m_pendingHotswaps;

    FW::FileWatcher* m_fileWatcher;
};
