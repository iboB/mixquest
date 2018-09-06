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

#include "mixins/Transform.mix.h"
#include "mixins/Character.h"

#include "systems/ActorSystem.h"

#include "messages/TransformMessages.h"

using namespace yama;

Game::Game(const MapDescription& mapDesc)
    : m_map(mapDesc)
{
    addSystem(m_actorSystem = new ActorSystem);
}

Game::~Game()
{
    for (auto system : m_systems)
    {
        delete system;
    }

    for (auto obj : m_objects)
    {
        delete obj;
    }
}

void Game::addSystem(GameplaySystem* system)
{
    assert(system);
    system->m_game = this;
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
        system->onObjectMutated(obj);
    }
}

dynamix::object& Game::spawnCharacter(int team, const std::string& name, ivector2* position)
{
    auto c = new dynamix::object;
    dynamix::mutate(c)
        .add<Transform>()
        .add<Character>();

    auto character = c->get<Character>();
    character->setName(name);
    character->setTeam(team);
    character->setMap(m_map);

    if (!position)
    {
        // find random empty square on the map to put the character
        while (true)
        {
            int x = rand() % m_map.size().x;
            int y = rand() % m_map.size().y;
            auto& cell = m_map.cell(x, y);
            if (cell.walkable && cell.objects.empty())
            {
                // suitable position found
                ::setPosition(c, vt(x, y));
                break;
            }
        }
    }
    else
    {
        ::setPosition(c, *position);
    }

    addObject(*c);
    m_map.addObject(*c, ::position(c));

    return *c;
}

void Game::addObject(dynamix::object& obj)
{
    m_objects.push_back(&obj);

    for (auto system : m_systems)
    {
        system->onObjectCreated(obj);
    }
}

const dynamix::object* Game::currentActor() const
{
    return m_actorSystem->currentObject();
}
