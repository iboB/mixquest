//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

DYNAMIX_DECLARE_MIXIN(CharacterRendering);

// looks like message but isn't
// it's another way of calling mixin-specific code without burdening the system with
// another message
void initializeCharacterRendering(dynamix::object& object);
