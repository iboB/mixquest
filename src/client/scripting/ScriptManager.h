//
// MixQuest
// Copyright (c) 2017-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/Common/ExplicitSingleton.h>

struct ScriptManager : public maibo::ExplicitSingleton<ScriptManager>
{
    MAIBO_DECLARE_EXPLICIT_SINGLETON(ScriptManager);
};
