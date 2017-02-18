//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// constants and initializers

namespace Vec
{

constexpr yama::vector3 up()
{
    return yama::vector3::coord(0, 0, 1);
}

constexpr yama::vector3 zero()
{
    return yama::vector3::zero();
}

}

namespace Mat
{

constexpr yama::matrix identity()
{
    return yama::matrix::identity();
}

}
