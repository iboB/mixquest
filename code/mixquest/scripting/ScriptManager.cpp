//
// MixQuest
// Copyright (c) 2017-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ScriptManager.h"

#include "gameplay/messages/ActorMessages.h"

#include <sol.hpp>

using namespace std;

ScriptManager::ScriptManager()
    : m_lua(new sol::state)
{
    //int x = 0;
    //lua.set_function("beep", [&x] { ++x; });
    //lua.script("beep()");
    //cout << x << endl;

    auto dmx = m_lua->create_named_table("dynamix");
    dmx.new_usertype<dynamix::object>("object",
        "executeCommand", (void(*)(dynamix::object*, Command)) executeCommand);

}

ScriptManager::~ScriptManager()
{
}

void ScriptManager::loadScript(const std::vector<char>& code)
{
    m_lua->script({ code.data(), code.size() });
}

void ScriptManager::testFunc(dynamix::object& obj)
{
    //executeCommand(obj, Command::Up);
    auto& lua = *m_lua;
    auto func = lua["TestFunc"];
    std::string data = func(&obj);
    puts(data.c_str());
}
