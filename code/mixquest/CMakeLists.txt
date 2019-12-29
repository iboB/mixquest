# MixQuest
# Copyright (c) 2017-2020 Borislav Stanimirov
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
    ${tp}/imgui/imgui_widgets.cpp
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
src_group("root" sources
    Main.cpp
    InGameState.h
    Application_ImGui.cpp
    ImplicitSingleton.h
    TaskManager.cpp
    TaskManager.h
    Memory.h
    LoadAllAssetsState.cpp
    OpenGL.h
    Application.cpp
    Util.h
    Application.h
    MixQuest.pch.cpp
    AppState.h
    CharacterNames.cpp
    Util.cpp
    LoadAllAssetsState.h
    Constants.h
    MixQuest.pch.h
    ExplicitSingleton.h
    CharacterNames.h
    InGameState.cpp
)
src_group("scripting" sources
    scripting/ScriptManager.h
    scripting/ScriptManager.cpp
)
src_group("control" sources
    control/CharacterControl.cpp
    control/CharacterControl.mix.h
)
src_group("map" sources
    map/MapRenderingTmx.cpp
    map/MapRendering.h
    map/MapRendering.cpp
)
src_group("assets" sources
    assets/GlobalAssets.cpp
    assets/TexturizeColorMaterial.h
    assets/LoadTMX.h
    assets/GlobalAssets.h
    assets/TexturizeMaterial.h
    assets/LoadTMX.cpp
    assets/TexturizeMaterial.cpp
    assets/TexturizeColorMaterial.cpp
)
src_group("mixins" sources
    mixins/SelectionMark.mix.h
    mixins/SelectionMark.cpp
    mixins/HealthBar.cpp
    mixins/KeyboardControl.mix.h
    mixins/HealthBar.mix.h
    mixins/CharacterRendering.mix.h
    mixins/CharacterRendering.cpp
    mixins/KeyboardControl.cpp
)
src_group("messages" sources
    messages/RenderingMessages.h
    messages/RenderingMessages.cpp
)
src_group("input" sources
    input/InputManager.h
)
src_group("plugins" sources
    plugins/PluginManager.h
    plugins/HotswapFSWatcher.cpp
    plugins/HotswapFSWatcher.h
    plugins/PluginManager.cpp
)
src_group("plugins~FileWatcher" sources
    plugins/FileWatcher/FileWatcher.cpp
    plugins/FileWatcher/FileWatcherImpl.h
    plugins/FileWatcher/FileWatcherWin32.h
    plugins/FileWatcher/FileWatcher.h
    plugins/FileWatcher/FileWatcherLinux.cpp
    plugins/FileWatcher/FileWatcherOSX.h
    plugins/FileWatcher/FileWatcherOSX.cpp
    plugins/FileWatcher/FileWatcherLinux.h
    plugins/FileWatcher/FileWatcherWin32.cpp
)
src_group("rendering" sources
    rendering/Texture.h
    rendering/SpriteAnimation.h
    rendering/Tileset.cpp
    rendering/TextureManager.cpp
    rendering/Sprite.h
    rendering/TextureManager.h
    rendering/Renderer.cpp
    rendering/GPUProgram.cpp
    rendering/Quad.h
    rendering/Texture.cpp
    rendering/Tileset.h
    rendering/Shader.h
    rendering/SpriteAnimation.cpp
    rendering/Sprite.cpp
    rendering/Renderer.h
    rendering/Vertex.h
    rendering/GPUProgram.h
    rendering/ShaderPtr.h
    rendering/GLSentries.h
    rendering/GPUProgramPtr.h
    rendering/ShaderType.h
    rendering/Shader.cpp
    rendering/Quad.cpp
    rendering/TexturePtr.h
)
src_group("systems" sources
    systems/RenderingSystem.h
    systems/RenderingSystem.cpp
)

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
