//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

DYNAMIX_EXPORTED_MESSAGE_1(GAMEPLAY_API, void, setPosition, const yama::ivector2&, pos);
DYNAMIX_EXPORTED_CONST_MESSAGE_0(GAMEPLAY_API, const yama::ivector2&, position);

enum class Direction
{
    East, North, West, South
};

DYNAMIX_EXPORTED_MESSAGE_1(GAMEPLAY_API, void, setDirection, Direction, dir);
DYNAMIX_EXPORTED_CONST_MESSAGE_0(GAMEPLAY_API, Direction, direction);

#define Transform_Interface setPosition_msg & position_msg & setDirection_msg & direction_msg
