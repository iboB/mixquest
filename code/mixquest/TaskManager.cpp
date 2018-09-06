//
// MixQuest
// Copyright (c) 2017-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "TaskManager.h"

TaskManager::TaskManager()
{
    m_taskIterator = m_tasks.end();
}

TaskManager::~TaskManager() = default;

void TaskManager::update()
{
    for (unsigned i = 0; i < m_numTasksPerUpdate; ++i)
    {
        if (m_tasks.empty())
            break;

        auto task = *m_taskIterator;

        if (task())
        {
            m_taskIterator = m_tasks.erase(m_taskIterator);
        }
        else
        {
            ++m_taskIterator;
        }

        if (m_taskIterator == m_tasks.end())
        {
            m_taskIterator = m_tasks.begin();
        }
    }
}

void TaskManager::pushTask(Task t)
{
    m_tasks.push_back(std::move(t));

    if (m_taskIterator == m_tasks.end())
    {
        m_taskIterator = m_tasks.begin();
    }
}

bool TaskManager::hasPendingTasks() const
{
    return m_tasks.end() != m_taskIterator;
}