#include "HotswapFSWatcher.h"

#include "PluginManager.h"
#include "FileWatcher/FileWatcher.h"

#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

namespace
{

class HotswapWatch : public FW::FileWatchListener
{
public:
    HotswapWatch(HotswapFSWatcher& watcher)
        : m_watcher(watcher)
    {}

    virtual void handleFileAction(FW::WatchID watchid, const std::string& dir, const std::string& filename, FW::Action action) override
    {
#if defined(_WIN32)
        if (action != FW::Actions::Modified) return; // we don't care for others
#else
        if (action != FW::Actions::Add) return;
#endif

        string fname = filename;

        // Extract only the filename without ./, which happens on Mac
        auto lastDirPos = filename.rfind('/');
        if (lastDirPos != string::npos)
        {
            fname = filename.substr(lastDirPos + 1);
        }

        m_watcher.addFileToHotswap(fname);
    }

    HotswapFSWatcher& m_watcher;
};

}

HotswapFSWatcher::HotswapFSWatcher(PluginManager& manager)
    : m_pluginManager(manager)
{
    m_fileWatcher = new FW::FileWatcher;
    m_fileWatcher->addWatch(m_pluginManager.buildOutputDir(), new HotswapWatch(*this));
}

HotswapFSWatcher::~HotswapFSWatcher()
{
    delete m_fileWatcher;
}

void HotswapFSWatcher::addFileToHotswap(const std::string& file)
{
    m_pendingHotswaps[file] = 300;
}

void HotswapFSWatcher::update(uint32_t deltams)
{
    m_fileWatcher->update();

    auto i = m_pendingHotswaps.begin();

    while (i != m_pendingHotswaps.end())
    {
        i->second -= deltams;
        if (i->second < 0)
        {
            m_pluginManager.onFileChanged(i->first);
            i = m_pendingHotswaps.erase(i);
        }
        else
        {
            ++i;
        }
    }
}