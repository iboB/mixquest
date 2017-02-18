//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ActorSystem.h"

#include "messages/ActorMessages.h"

void ActorSystem::update(uint32_t dms)
{
    if (m_objects.empty())
    {
        m_currentObject = nullptr;
    }
    else
    {
        if (!m_currentObject)
        {
            m_currentObject = m_objects.front();
        }

        if (decideActorActions(m_currentObject))
        {
            // object successfully performed actions
            // select next
            selectNextObject();
        }
    }
}

bool ActorSystem::checkObject(dynamix::object& object) const
{
    return object.implements(decideActorActions_msg);
}

bool ActorSystem::onNewObject(dynamix::object& object)
{
    // take care of turns if any
    return true;
}

bool ActorSystem::onRemovingObject(dynamix::object& object, bool force)
{
    if (m_currentObject == &object)
    {
        if (m_objects.size() == 1)
        {
            m_currentObject = nullptr;
        }

        selectNextObject();
    }
    return true;
}

void ActorSystem::selectNextObject()
{
    assert(!m_objects.empty());

    for (auto i = m_objects.begin(); i != m_objects.end(); ++i)
    {
        if (*i == m_currentObject)
        {
            ++i;
            if (i == m_objects.end())
            {
                i = m_objects.begin();
            }

            m_currentObject = *i;
            return;
        }
    }

    // should never get here
    assert(false);

}
