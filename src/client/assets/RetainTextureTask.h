//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <maibo/Task.h>
#include <maibo/Common/ResourceFuture.h>
#include <maibo/Rendering/TexturePtr.h>

class RetainTextureTask : public maibo::Task
{
public:
    RetainTextureTask(const std::string& name, maibo::ConstResourceFuturePtr<maibo::TexturePtr> depFuture);

    virtual bool execute() override;

    // used for the wait state
    maibo::ConstFuturePtr future() const { return m_future; }

public:
    std::string m_name;
    maibo::ConstResourceFuturePtr<maibo::TexturePtr> m_depFuture;
    maibo::FuturePtr m_future; // used for the wait state
};
