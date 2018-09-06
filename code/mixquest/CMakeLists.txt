# MixQuest
# Copyright (c) 2017-2018 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# http://opensource.org/licenses/MIT
#

# deps
set(tp ${mq_root}/third_party)

add_library(glad STATIC
    ${tp}/glad/src/glad.c)
target_include_directories(glad
    PUBLIC ${tp}/glad/include)
set_target_properties(glad PROPERTIES FOLDER ThirdParty)

add_library(imgui STATIC
    ${tp}/imgui/imgui.cpp
    ${tp}/imgui/imgui_demo.cpp
    ${tp}/imgui/imgui_draw.cpp
)
target_include_directories(imgui
    INTERFACE ${tp}/imgui)
set_target_properties(imgui PROPERTIES FOLDER ThirdParty)

add_subdirectory(${tp}/tmxparser tmxparser)
set_target_properties(tmxparser PROPERTIES FOLDER ThirdParty)

add_library(stb INTERFACE)
target_include_directories(stb
    INTERFACE ${tp}/stb
)

add_library(sol INTERFACE)
target_include_directories(sol
    INTERFACE ${tp}/sol2
)

add_subdirectory(${tp}/lua lua)
set_target_properties(lua PROPERTIES FOLDER ThirdParty)

# exe
set(sources)
ucm_add_dirs(. TO sources RECURSIVE FILTER_POP 1)

add_executable(mixquest
    "${sources}"
)

set(inc)
set(libs)

if(WIN32)
    set(inc
        ${inc}
        ${mq_root}/vendor/glfw-3.2.1/include
    )
    set(libs
        ${libs}
        ${mq_root}/vendor/glfw-3.2.1/build/src/Release/glfw3.lib
    )

    target_compile_definitions(mixquest PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
else()
    set(libs
        ${libs}
        glfw
        dl
    )
endif()

target_include_directories(mixquest
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${inc})

target_link_libraries(mixquest
    gameplay
    ${libs}
    glad
    imgui
    tmxparser
    stb
    sol
    lua
)

if(NOT mq_shared)
    target_link_libraries(mixquest ai)
endif()

mq_target_properties(mixquest)

add_precompiled_header(mixquest ${CMAKE_CURRENT_SOURCE_DIR}/MixQuest.pch.h ${CMAKE_CURRENT_SOURCE_DIR}/MixQuest.pch.cpp)
