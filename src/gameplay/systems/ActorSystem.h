//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "GameplaySystem.h"

class ActorSystem : public GameplaySystem
{
public:
    virtual void update(uint32_t dms) override;
    virtual bool checkObject(dynamix::object& object) const override;

    const dynamix::object* currentObject() const { return m_currentObject; }

private:
    virtual bool onNewObject(dynamix::object& object) override;
    virtual bool onRemovingObject(dynamix::object& object, bool force) override;

    void selectNextObject();

    // object whose turn it is now
    dynamix::object* m_currentObject = nullptr;
};
