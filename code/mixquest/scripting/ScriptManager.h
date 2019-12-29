//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "ExplicitSingleton.h"

namespace sol
{
class state;
}

struct ScriptManager : public ExplicitSingleton<ScriptManager>
{
    DECLARE_EXPLICIT_SINGLETON(ScriptManager);

public:
    void loadScript(const std::vector<char>& code);
    void testFunc(dynamix::object& obj);

private:
    std::unique_ptr<sol::state> m_lua;
};
