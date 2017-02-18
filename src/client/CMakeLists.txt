cmake_minimum_required(VERSION 3.2)

project(mixquest)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MAIBO_INCLUDE_DIRS}
    ${DYNAMIX_INCLUDE}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/tmxparser/src
)

link_directories(
    ${MAIBO_LIB_DIRS}
)

if(MIXQUEST_USE_SHARED_LIBS)
    add_definitions(-DGAMEPLAY_SHARED)
endif()

set(sources)
ucm_add_dirs(. TO sources RECURSIVE FILTER_POP 1)

maibo_add_executable(mixquest
    "${sources}"
)

target_link_libraries(mixquest
    tmxparser
    gameplay
    dynamix
    ${MAIBO_LIBS}
)

add_precompiled_header(mixquest ${CMAKE_CURRENT_SOURCE_DIR}/${SRC}/MixQuest.pch.h ${CMAKE_CURRENT_SOURCE_DIR}/${SRC}/MixQuest.pch.cpp)

if(MSVC)
    set(USERFILE_WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
    set(THIRD_PARTY_LIB)
    file(TO_NATIVE_PATH "${MAIBO_ROOT_DIR}/third_party/lib/prebuilt/Windows" THIRD_PARTY_LIB)

    configure_file(
        "${MAIBO_ROOT_DIR}/build/tools/ExampleExe.template.user"
        "${CMAKE_CURRENT_BINARY_DIR}/mixquest.vcxproj.user"
        @ONLY
    )
endif()

