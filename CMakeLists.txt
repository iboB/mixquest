# MixQuest
# Copyright (c) 2017-2020 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# http://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.2)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(mixquest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(mq_root ${CMAKE_CURRENT_SOURCE_DIR})
set(mq_shared YES)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${mq_root}/third_party/cmake
)

include(precompiled_header)
include(src_group)

macro(mq_target_properties tgt)
    set_target_properties(${tgt}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${mq_root}/bin"
        # ARCHIVE_OUTPUT_DIRECTORY "${mq_root}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${mq_root}/bin"
    )
endmacro(mq_target_properties)

add_subdirectory(code/gameplay)

add_subdirectory(code/plugins/ai)

add_subdirectory(code/mixquest)
