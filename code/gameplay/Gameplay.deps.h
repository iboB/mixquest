#pragma once

#include <dynamix/core.hpp>

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

#if defined(_MSC_VER)
// msvc complains that template classes don't have a dll interface (they shouldn't).
#   pragma warning (disable: 4251)
#endif

#include <vector>

#include <yama/yama.hpp>

namespace yama
{
using ivector2 = vector2_t<int>;
using uvector2 = vector2_t<uint32_t>;
}
