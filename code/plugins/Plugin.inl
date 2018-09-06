//
// MixQuest
// Copyright (c) 2017 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#undef PLUGIN_FUNC

#if defined(NO_PLUGINS)
#   define PLUGIN_FUNC(ret, func, args) extern ret func args
#elif defined(PLUGIN_SRC)
#   define PLUGIN_FUNC(ret, func, args) SYMBOL_EXPORT ret func args
#elif defined(PLUGIN_DEF)
#   define PLUGIN_FUNC(ret, func, args) ret (*func) args
#elif defined(PLUGIN_LOAD)
#   define PLUGIN_FUNC(ret, func, args) func = (ret (*) args)GetProc(dynlib, #func)
#elif defined(PLUGIN_INC)
#   define PLUGIN_FUNC(ret, func, args) extern ret (*func) args
#else
#   error Unknown plugin config
#endif
