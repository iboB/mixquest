//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

class GameplaySystem;
struct MapDescription;

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


private:
    std::vector<GameplaySystem*> m_systems;

    std::vector<dynamix::object> m_objects;
};