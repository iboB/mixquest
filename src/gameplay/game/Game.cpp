//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Game.h"

#include "systems/GameplaySystem.h"

Game::Game(const MapDescription& mapDesc)
{

}

Game::~Game()
{
    for (auto system : m_systems)
    {
        delete system;
    }
}

void Game::addSystem(GameplaySystem* system)
{
    assert(system);
    m_systems.push_back(system);
}

void Game::removeSystem(GameplaySystem* system)
{
    auto isys = std::find(m_systems.begin(), m_systems.end(), system);
    assert(isys != m_systems.end());
    delete *isys;
    m_systems.erase(isys);
}

void Game::update(uint32_t dms)
{
    for (auto system : m_systems)
    {
        system->update(dms);
    }
}

void Game::onObjectMutated(dynamix::object& obj)
{
    for (auto system : m_systems)
    {
        system->onObjectCreated(obj);
    }
}