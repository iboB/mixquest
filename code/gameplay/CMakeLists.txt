# MixQuest
# Copyright (c) 2017-2020 Borislav Stanimirov
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
src_group("root" sources
    Gameplay.deps.h
    Gameplay.pch.h
    Gameplay.pch.cpp
)
src_group("mixins" sources
    mixins/Character.cpp
    mixins/Character.mix.h
    mixins/SelectedActor.cpp
    mixins/SelectedActor.mix.h
    mixins/Character.h
    mixins/Transform.cpp
    mixins/Transform.mix.h
)
src_group("messages" sources
    messages/CharacterMessages.cpp
    messages/TransformMessages.h
    messages/CharacterMessages.h
    messages/TransformMessages.cpp
    messages/ActorMessages.cpp
    messages/ActorMessages.h
)
src_group("game" sources
    game/MapDescription.h
    game/Map.h
    game/Game.h
    game/Map.cpp
    game/Game.cpp
    game/MapDescription.cpp
)
src_group("systems" sources
    systems/GameplaySystem.cpp
    systems/ActorSystem.h
    systems/ActorSystem.cpp
    systems/GameplaySystem.h
)

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
