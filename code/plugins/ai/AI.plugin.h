//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "plugins/Plugin.inl"

#if defined(PLUGIN_DEF)
class Game;
#endif

PLUGIN_FUNC(void, AIPluginLoad, (Game* game));
PLUGIN_FUNC(void, AIPluignUnload, ());
