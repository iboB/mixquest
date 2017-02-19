//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "RetainTextureTask.h"

#include <maibo/Rendering/TextureManager.h>

using namespace maibo;

RetainTextureTask::RetainTextureTask(const std::string& name, ConstResourceFuturePtr<TexturePtr> depFuture)
    : m_name(name)
    , m_depFuture(depFuture)
    , m_future(new Future)
{
}

bool RetainTextureTask::execute()
{
    if (m_depFuture)
    {
        if (!m_depFuture->isDone())
        {
            // wait for dependent future
            return false;
        }

        if (m_depFuture->errorCode())
        {
            m_future->setDone();
            m_future->setErrorCode(1);
            m_future->setProgress(1.f);
            return true;
        }
    }


    auto& tm = TextureManager::instance();
    tm.addTexture(m_depFuture->resource(), m_name, true);

    m_future->setDone();
    m_future->setErrorCode(0);
    m_future->setProgress(1.f);

    return true;
}