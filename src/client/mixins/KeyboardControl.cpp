//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "KeyboardControl.mix.h"

#include "input/InputManager.h"

#include "gameplay/messages/ActorMessages.h"
#include "gameplay/messages/TransformMessages.h"

class KeyboardControl
{
public:
    bool decideActorActions()
    {
        auto command = InputManager::instance().pendingCommand();
        if (command == Command::None) return false;

        if (canExecuteCommand(dm_this, command))
        {
            executeCommand(dm_this, command);
            return true;
        }

        return false;
    }
};

DYNAMIX_DEFINE_MIXIN(KeyboardControl, decideActorActions_msg);
