//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/Task.h>
#include <maibo/Common/ResourceFuture.h>

namespace maibo
{
class WaitForFuturesState;
}

namespace Tmx
{
class Map;
}

// class that loads the map description from a tmx file

class LoadTMXTask : public maibo::Task
{
public:
    LoadTMXTask(maibo::ConstResourceFuturePtr<std::vector<char>> readTMX, maibo::WaitForFuturesState& state);

    virtual bool execute() override;

    maibo::ResourceFuturePtr<std::shared_ptr<Tmx::Map>> future;

private:

    maibo::ConstResourceFuturePtr<std::vector<char>> m_readTMXFuture;
    maibo::WaitForFuturesState& m_waitState;
    int m_errorCode = 0;
};
