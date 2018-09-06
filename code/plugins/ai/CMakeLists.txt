# MixQuest
# Copyright (c) 2017-2018 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# http://opensource.org/licenses/MIT
#

set(sources)
ucm_add_dirs(. TO sources RECURSIVE FILTER_POP 1)

if(mq_shared)
    add_library(ai SHARED
        "${sources}"
    )

    target_compile_definitions(ai 
        PRIVATE -DPLUGIN_SRC)

    target_link_libraries(ai
        PRIVATE gameplay
    )
    if (NOT MSVC)
        target_compile_options(ai 
            PRIVATE "-fPIC"
        )
    endif()
else()
    add_library(ai STATIC
        "${sources}"
    )
endif()

mq_target_properties(ai)

add_precompiled_header(ai ${CMAKE_CURRENT_SOURCE_DIR}/AI.pch.h ${CMAKE_CURRENT_SOURCE_DIR}/AI.pch.cpp)

set_target_properties(ai PROPERTIES FOLDER Plugins)
