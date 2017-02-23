//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

DYNAMIX_EXPORTED_MESSAGE_0(GAMEPLAY_API, bool, decideActorActions);

enum class Command
{
    None, Down, Right, Up, Left, SelfHarm,
};

DYNAMIX_EXPORTED_CONST_MESSAGE_1(GAMEPLAY_API, bool, canExecuteCommand, Command, c);
DYNAMIX_EXPORTED_MESSAGE_1(GAMEPLAY_API, void, executeCommand, Command, c);
