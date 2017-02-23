# MixQuest
# Copyright (c) 2017 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# http://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.2)

project(mixquest)

# Set maibo build options
set(MAIBO_USE_IMGUI ON
    CACHE BOOL "ImGui third party library"
)
set(MAIBO_BUILD_EXAMPLES OFF
    CACHE BOOL "Build maibo examples"
)

# Add maibo
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/maibo)
set_target_properties(maibo PROPERTIES FOLDER MaiBo)

# emscripten doesn't add libcxxabi to the default include paths since not all
# features are supported
# however the ones we use, are
if(MAIBO_PLATFORM_EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem${EMSCRIPTEN_ROOT_PATH}/system/lib/libcxxabi/include")
endif()

if(MAIBO_PLATFORM_DESKTOP AND NOT MIXQUEST_FORCE_STATIC_LIBS)
    set(MIXQUEST_USE_SHARED_LIBS 1)
else()
    set(MIXQUEST_USE_SHARED_LIBS 0)
endif()

if(MIXQUEST_USE_SHARED_LIBS)
    set(DYNAMIX_SHARED_LIB ON CACHE BOOL "dynamix shared")
else()
    set(DYNAMIX_SHARED_LIB OFF CACHE BOOL "dynamix shared")
endif()
set(DYNAMIX_BUILD_UNIT_TESTS OFF CACHE BOOL "dynamix no unit tests")
set(DYNAMIX_BUILD_EXAMPLES OFF CACHE BOOL "dynamix no examples")
set(DYNAMIX_BUILD_SCRATCH OFF CACHE BOOL "dynamix no scratch")
set(DYNAMIX_BUILD_TUTORIALS OFF CACHE BOOL "dynamix no tutorials")
set(DYNAMIX_BUILD_PERF OFF CACHE BOOL "dynamix no perf")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/dynamix)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/tmxparser)
set_target_properties(tmxparser PROPERTIES FOLDER ThirdParty)

maibo_set_c_cxx_flags()

if(MIXQUEST_USE_SHARED_LIBS)
    if(MSVC)
        add_definitions(/wd4251)
    endif()

    add_definitions(-DDYNAMIX_DYNLIB)
else()
    add_definitions(-DNO_PLUGINS)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/gameplay)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/ai)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/client)

maibo_target_properties(dynamix)
maibo_target_properties(gameplay)
maibo_target_properties(mixquest)
maibo_target_properties(ai)
