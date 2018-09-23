//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "RenderingSystem.h"
#include "messages/RenderingMessages.h"

#include "mixins/CharacterRendering.mix.h"
#include "mixins/HealthBar.mix.h"

#include "gameplay/mixins/Character.h"
#include "gameplay/messages/CharacterMessages.h"

RenderingSystem::RenderingSystem(Renderer& renderer)
    : m_renderer(renderer)
{

}

void RenderingSystem::update(uint32_t dms)
{
    for (auto o : m_objects)
    {
        supplyRenderingParts(o, m_renderer);
    }
}

bool RenderingSystem::checkObject(dynamix::object& object) const
{
    // Play the role of a factory for bundling mixins with their rendering counterparts
    // we don't call onObjectMutated since we assume we're the only one that cares
    // about the newly added mutations here
    // Someday this might turn out not to be case...

    if (object.has<Character>())
    {
        if (!object.has<CharacterRendering>())
        {
            dynamix::mutate(object).add<CharacterRendering>();
            initializeCharacterRendering(object);
        }
    }

//    if (object.implements(hp_msg))
//    {
//        if (!object.has<HealthBar>())
//        {
//            dynamix::mutate(object).add<HealthBar>();
//            initializeHealthBar(object);
//        }
//    }

    return object.implements(supplyRenderingParts_msg);
}
