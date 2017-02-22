//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

DYNAMIX_EXPORTED_CONST_MESSAGE_0(GAMEPLAY_API, int, team);
DYNAMIX_EXPORTED_CONST_MESSAGE_0(GAMEPLAY_API, int, hp);
DYNAMIX_EXPORTED_CONST_MESSAGE_0(GAMEPLAY_API, int, maxHP);
DYNAMIX_EXPORTED_CONST_MESSAGE_0(GAMEPLAY_API, const std::string&, name);

#define Character_Interface team_msg & name_msg & hp_msg & maxHP_msg
