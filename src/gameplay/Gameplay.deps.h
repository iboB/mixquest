#pragma once

#include <dynamix/dynamix.hpp>

#if !defined(SYMBOL_IMPORT)
#   if defined(_MSC_VER)
#       define SYMBOL_EXPORT __declspec(dllexport)
#       define SYMBOL_IMPORT __declspec(dllimport)
#   else
#       define SYMBOL_EXPORT __attribute__((__visibility__("default")))
#       define SYMBOL_IMPORT
#   endif
#endif


#if defined(GAMEPLAY_SHARED)
#   if defined(GAMEPLAY_SRC)
#       define GAMEPLAY_API SYMBOL_EXPORT
#   else
#       define GAMEPLAY_API SYMBOL_IMPORT
#   endif
#else
#   define GAMEPLAY_API
#endif

#include <vector>

#include <yama/yama.hpp>

namespace yama
{
using ivector2 = yama::vector2_t<int>;
}
