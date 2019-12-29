//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <list>
#include <functional>

class TaskManager
{
public:
    TaskManager();
    ~TaskManager();

    using Task = std::function<bool()>;

    // sets how many calls of Task::execute should there ber per TaskManager::update
    void setNumTasksPerUpdate(unsigned numTasksPerUpdate) { m_numTasksPerUpdate = numTasksPerUpdate; }

    // called each frame to execute m_numTasksPerUpdate takss
    void update();

    void pushTask(Task task);

    bool hasPendingTasks() const;

private:
    std::list<Task> m_tasks;
    std::list<Task>::iterator m_taskIterator;

    unsigned m_numTasksPerUpdate = 1;

};
