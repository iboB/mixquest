cmake_minimum_required(VERSION 3.2)

project(mixquest-plugin-ai)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../maibo/third_party/lib/yama/include
)

set(sources)
ucm_add_dirs(. TO sources RECURSIVE FILTER_POP 1)

if(MIXQUEST_USE_SHARED_LIBS)
    add_definitions(-DGAMEPLAY_SHARED -DPLUGIN_SRC)

    add_library(ai SHARED
        "${sources}"
    )

    target_link_libraries(ai
        gameplay
        dynamix
    )

    target_compile_options(ai PRIVATE
        "-fPIC"
    )
else()
    add_library(ai STATIC
        "${sources}"
    )
endif()

add_precompiled_header(ai ${CMAKE_CURRENT_SOURCE_DIR}/${SRC}/AI.pch.h ${CMAKE_CURRENT_SOURCE_DIR}/${SRC}/AI.pch.cpp)
set_target_properties(ai PROPERTIES FOLDER Plugins)
