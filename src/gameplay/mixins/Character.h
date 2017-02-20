//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "Character.mix.h"

#include "../messages/ActorMessages.h"

class Map;

class GAMEPLAY_API Character
{
public:
    Character();

    const std::string& name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
    void setTeam(int team) { m_team = team; }
    int team() const { return m_team; }
    int hp() const { return m_hp; }
    void setMap(Map& map);

    bool canExecuteCommand(Command cmd) const;
    void executeCommand(Command cmd);

private:
    bool canMoveTo(const yama::ivector2& pos) const;
    void moveTo(const yama::ivector2& pos);

    yama::ivector2 newPosFromCommand(Command cmd) const;

    std::string m_name;
    int m_team = 0;
    int m_hp = 0;
    Map* m_map = nullptr;
};


