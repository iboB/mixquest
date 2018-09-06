# MixQuest
# Copyright (c) 2017-2018 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# http://opensource.org/licenses/MIT
#

# deps 
if(mq_shared)
    set(DYNAMIX_SHARED_LIB ON CACHE BOOL "dynamix shared")
else()
    set(DYNAMIX_SHARED_LIB OFF CACHE BOOL "dynamix shared")
endif()
add_subdirectory(${mq_root}/third_party/dynamix dynamix)
mq_target_properties(dynamix)

add_library(yama INTERFACE)
target_include_directories(yama 
    INTERFACE ${mq_root}/third_party/yama/include
)

# gameplay target
set(sources)
ucm_add_dirs(. TO sources RECURSIVE FILTER_POP 1)

if(mq_shared)
    add_library(gameplay SHARED
        "${sources}"
    )

    target_compile_definitions(gameplay 
        PUBLIC -DGAMEPLAY_SHARED
        PRIVATE -DGAMEPLAY_SRC
    )

    if (NOT MSVC)
        target_compile_options(gameplay
            PRIVATE "-fPIC"
        )
    endif()
else()
    add_library(gameplay STATIC
        "${sources}"
    )
endif()

mq_target_properties(gameplay)

target_include_directories(gameplay 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gameplay
    PUBLIC dynamix yama
)

add_precompiled_header(gameplay ${CMAKE_CURRENT_SOURCE_DIR}/Gameplay.pch.h ${CMAKE_CURRENT_SOURCE_DIR}/Gameplay.pch.cpp)
