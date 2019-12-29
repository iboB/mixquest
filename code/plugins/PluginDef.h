//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#if defined(NO_PLUGINS)
#   define PLUGIN_DEF_BEGIN
#   define PLUGIN_DEF_END
#else
#   define PLUGIN_DEF_BEGIN extern "C" {
#   define PLUGIN_DEF_END }
#endif
