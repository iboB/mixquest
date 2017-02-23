//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "AI.mix.h"

#include "gameplay/messages/ActorMessages.h"
#include "gameplay/messages/CharacterMessages.h"

class AI
{
public:
    bool decideActorActions()
    {
        auto cmd = Command::Left;
        auto alt = Command::Right;

        if (::canExecuteCommand(dm_this, cmd))
        {
            ::executeCommand(dm_this, cmd);
        }
        else if(::canExecuteCommand(dm_this, alt))
        {
            ::executeCommand(dm_this, alt);
        }
        else
        {
            ::executeCommand(dm_this, Command::SelfHarm);
        }

        return true;
    }
};

DYNAMIX_DEFINE_MIXIN(AI, dynamix::priority(1, decideActorActions_msg));
