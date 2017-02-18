#include "GameplaySystem.h"

GameplaySystem::GameplaySystem()
{

}

GameplaySystem::~GameplaySystem()
{

}

void GameplaySystem::onObjectCreated(dynamix::object& object)
{
    if (checkObject(object))
    {
        if (onNewObject(object))
        {
            m_objects.push_back(&object);
        }
    }
}

void GameplaySystem::onDestroyingObject(dynamix::object& object)
{
    if (checkObject(object))
    {
        auto iobj = std::find(m_objects.begin(), m_objects.end(), &object);
        assert(iobj != m_objects.end());
        onRemovingObject(object, true);
        m_objects.erase(iobj);
    }
#if !defined(NDEBUG)
    // debug only check that if the object doesn't belong in this system it's not there
    else
    {
        auto iobj = std::find(m_objects.begin(), m_objects.end(), &object);
        assert(iobj == m_objects.end());
    }
#endif
}

void GameplaySystem::onObjectMutated(dynamix::object& object)
{
    if (checkObject(object))
    {
        auto iobj = std::find(m_objects.begin(), m_objects.end(), &object);
        if (iobj == m_objects.end())
        {
            if (onNewObject(object))
            {
                // objects now belongs here
                m_objects.push_back(&object);
            }
        }
    }
    else
    {
        auto iobj = std::find(m_objects.begin(), m_objects.end(), &object);
        if (iobj != m_objects.end())
        {
            if (onRemovingObject(object, false))
            {
                // relevant stuff has been mutated out of this object
                m_objects.erase(iobj);
            }
        }
    }
}