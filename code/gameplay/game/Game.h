//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "Map.h"

class GameplaySystem;
struct MapDescription;
class ActorSystem;

class GAMEPLAY_API Game
{
public:
    Game(const MapDescription& mapDesc);
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void update(uint32_t dms);

    void onObjectMutated(dynamix::object& obj);

    void addSystem(GameplaySystem* system);
    void removeSystem(GameplaySystem* system);

    dynamix::object& spawnCharacter(int team, const std::string& name, yama::ivector2* position = nullptr);

    const Map& map() const { return m_map; }
    Map& map() { return m_map; }

    const dynamix::object* currentActor() const;

    std::vector<dynamix::object*>& modifyObjects() { return m_objects; }

private:
    void addObject(dynamix::object& obj);

    Map m_map;

    std::vector<GameplaySystem*> m_systems;

    std::vector<dynamix::object*> m_objects;

    ActorSystem* m_actorSystem = nullptr;
};