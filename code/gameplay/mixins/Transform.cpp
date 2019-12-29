//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Transform.mix.h"
#include "messages/TransformMessages.h"

class Transform
{
public:
    void setPosition(const yama::ivector2& pos)
    {
        m_position = pos;
    }

    const yama::ivector2& position() const
    {
        return m_position;
    }

    void setDirection(const Direction dir)
    {
        m_direction = dir;
    }

    Direction direction() const
    {
        return m_direction;
    }

    yama::ivector2 m_position;
    Direction m_direction;
};

DYNAMIX_DEFINE_MIXIN(Transform, Transform_Interface)