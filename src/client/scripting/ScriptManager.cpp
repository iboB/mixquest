//
// MixQuest
// Copyright (c) 2017-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ScriptManager.h"

#include <sol/sol.hpp>

using namespace std;

ScriptManager::ScriptManager()
{
    sol::state lua;
    int x = 0;
    lua.set_function("beep", [&x] { ++x; });
    lua.script("beep()");
    cout << x << endl;
}

ScriptManager::~ScriptManager()
{
}