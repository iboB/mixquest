//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/Common/ImplicitSingleton.h>
#include "gameplay/messages/ActorMessages.h"

class InputManager : public maibo::ImplicitSingleton<InputManager>
{
public:

    void setCommand(Command c) { m_command = c; }
    Command pendingCommand() const { return m_command; }

private:
    Command m_command = Command::None;
};
