//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Character.h"
#include "messages/CharacterMessages.h"
#include "messages/ActorMessages.h"
#include "messages/TransformMessages.h"

#include "game/Map.h"

using namespace yama;

Character::Character()
{
}

bool Character::canExecuteCommand(Command cmd) const
{
    auto pos = newPosFromCommand(cmd);

    return canMoveTo(pos);
}

void Character::executeCommand(Command cmd)
{
    auto pos = newPosFromCommand(cmd);

    return moveTo(pos);
}

ivector2 Character::newPosFromCommand(Command cmd) const
{
    auto pos = ::position(dm_this);

    switch (cmd)
    {
    case Command::Down:
        pos.y -= 1;
        break;
    case Command::Right:
        pos.x += 1;
        break;
    case Command::Up:
        pos.y += 1;
        break;
    case Command::Left:
        pos.x -= 1;
        break;
    default:
        break;
    }

    return pos;
}

bool Character::canMoveTo(const ivector2& pos) const
{
    auto& cell = m_map->cell(pos);
    return cell.walkable && cell.objects.empty();
}

void Character::moveTo(const ivector2& pos)
{
    assert(canMoveTo(pos));
    m_map->moveObject(*dm_this, ::position(dm_this), pos);
    ::setPosition(dm_this, pos);
}

void Character::setMap(Map& map)
{
    assert(!m_map);
    m_map = &map;
}

DYNAMIX_DEFINE_MIXIN(Character, Character_Interface & canExecuteCommand_msg & executeCommand_msg);

