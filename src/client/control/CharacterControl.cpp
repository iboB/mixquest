//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//

#include "CharacterControl.mix.h"
#include "gameplay/messages/ActorMessages.h"

// mixin for controling a character

class CharacterControl
{
public:
    bool decideActorActions()
    {
        return true;
    }
};

DYNAMIX_DEFINE_MIXIN(CharacterControl, decideActorActions_msg);
