#include "plugins/PluginDef.h"

#include "gameplay/game/Game.h"
#include "gameplay/systems/GameplaySystem.h"
#include "gameplay/mixins/Character.mix.h"
#include "gameplay/messages/CharacterMessages.h"

#include "AI.mix.h"

#include <iostream>

using namespace std;

class AISystem : public GameplaySystem
{
public:
    ~AISystem()
    {
        for (auto obj : m_objects)
        {
            assert(obj->has<AI>());
            dynamix::mutate(obj).remove<AI>();
        }
    }

    virtual void update(uint32_t dms) {}
    virtual bool checkObject(dynamix::object& object) const
    {
        if (object.has<Character>())
        {
            bool evil = ::team(object) == 1;
            if (!object.has<AI>() && evil)
            {
                dynamix::mutate(object).add<AI>();
            }
            return evil;
        }

        return false;
    }

};

AISystem* theSystem = nullptr;
Game* theGame = nullptr; // you lost it

PLUGIN_DEF_BEGIN

#include "AI.plugin.h"

void AIPluginLoad(Game* game)
{
    theGame = game;
    theSystem = new AISystem;
    game->addSystem(theSystem);
    auto& objects = game->modifyObjects();
    for (auto obj : objects)
    {
        theSystem->onObjectCreated(*obj);
    }
}

void AIPluignUnload()
{
    theGame->removeSystem(theSystem);
    theGame = nullptr;
    theSystem = nullptr;
}

PLUGIN_DEF_END
