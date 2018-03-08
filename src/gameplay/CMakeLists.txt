cmake_minimum_required(VERSION 3.2)

project(mixquest-gameplay)

set(sources)
ucm_add_dirs(. TO sources RECURSIVE FILTER_POP 1)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../maibo/third_party/lib/yama/include
)

if(MIXQUEST_USE_SHARED_LIBS)
    add_definitions(-DGAMEPLAY_SHARED -DGAMEPLAY_SRC)

    add_library(gameplay SHARED
        "${sources}"
    )

    target_link_libraries(gameplay
        dynamix
    )

    target_compile_options(gameplay PRIVATE
        "-fPIC"
    )
else()
    add_library(gameplay STATIC
        "${sources}"
    )
endif()

add_precompiled_header(gameplay ${CMAKE_CURRENT_SOURCE_DIR}/Gameplay.pch.h ${CMAKE_CURRENT_SOURCE_DIR}/Gameplay.pch.cpp)
