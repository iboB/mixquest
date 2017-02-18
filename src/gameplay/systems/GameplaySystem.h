#pragma once

class GAMEPLAY_API GameplaySystem
{
public:
    GameplaySystem();
    virtual ~GameplaySystem();

    GameplaySystem(const GameplaySystem&) = delete;
    GameplaySystem& operator=(const GameplaySystem&) = delete;

    virtual void update(uint32_t dms) = 0;

    // object management
    // checks if the object belongs in this system
    // potentially make changes to the object, too
    virtual bool checkObject(dynamix::object& object) const = 0;

    void onObjectCreated(dynamix::object& object);
    void onDestroyingObject(dynamix::object& object);
    void onObjectMutated(dynamix::object& object);

protected:

    // called when a new object is about to be pushed back to the system's object list
    // return true to accept the add or false to block it
    virtual bool onNewObject(dynamix::object& object) { return true; }

    // called when an object is about to be removed from the the system's object list
    // return false to block the removal unless force is true
    // force means that the return value will be disregarded and the object is going to be removed
    virtual bool onRemovingObject(dynamix::object& object, bool force) { return true; }

    std::vector<dynamix::object*> m_objects;
};