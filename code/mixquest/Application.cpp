//
// MixQuest
// Copyright (c) 2017-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Application.h"
#include "AppState.h"

#include "OpenGL.h"

#include "rendering/TextureManager.h"
#include "assets/GlobalAssets.h"
#include "scripting/ScriptManager.h"

#include "LoadAllAssetsState.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <chrono>
#include <cassert>

#include <imgui.h>

#if !defined(__EMSCRIPTEN__)
#   include <thread>
#endif

#if defined(_WIN32)
#   include <Windows.h>
#elif !defined(__EMSCRIPTEN__)
#   include <dlfcn.h>
#endif

using namespace std;

Application::Application() = default;
Application::~Application() = default;

Application::CreationParameters::CreationParameters() = default;

namespace
{
void getAddr() {}

Application& AppFrom(GLFWwindow* window)
{
    auto ptr = glfwGetWindowUserPointer(window);
    assert(ptr);
    auto app = reinterpret_cast<Application*>(ptr);
    return *app;
}
}

extern "C"
{
    static void glfw_error_callback(int error, const char* description)
    {
        cerr << "GLFW error: " << description << '\n';
    }

    static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        AppFrom(window).onKeyboardEvent(key, action, mods);
    }

    static void glfw_character_callback(GLFWwindow* window, unsigned int codepoint)
    {
        AppFrom(window).onCharacterEvent(codepoint);
    }

    static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        AppFrom(window).onMouseButtonEvent(button, action, mods);
    }

    void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        AppFrom(window).onScrollEvent(xoffset, yoffset);
    }
}

bool Application::initialize(Application::CreationParameters cp)
{
    m_currentFrameTime = getTicks();

    if (!glfwInit())
    {
        cerr << "Couldn't init glfw\n";
        return false;
    }
    glfwSetErrorCallback(glfw_error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    m_window = glfwCreateWindow(
        cp.mainWindowClientAreaSize.x,
        cp.mainWindowClientAreaSize.y,
        cp.mainWindowTitle,
        nullptr,
        nullptr);

    if (!m_window)
    {
        cerr << "Couldn't create GLFW window\n";
        return false;
    }

    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, 1);
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, 1);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, glfw_key_callback);
    glfwSetCharCallback(m_window, glfw_character_callback);
    glfwSetMouseButtonCallback(m_window, glfw_mouse_button_callback);
    glfwSetScrollCallback(m_window, glfw_scroll_callback);

    glfwMakeContextCurrent(m_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    m_desiredFrameTime = cp.desiredFrameTimeMs;

#if defined(_WIN32)
    HMODULE engine;
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCSTR)getAddr,
        &engine);

    char path[_MAX_PATH + _MAX_FNAME + 1];
    GetModuleFileNameA(engine, path, _MAX_PATH + _MAX_FNAME);

    // normalize path
    char* p = path;
    while (*p)
    {
        if (*p == '\\')
        {
            *p = '/';
        }
        ++p;
    }

    m_fileName = path;

#elif !defined(__EMSCRIPTEN__)
    void* p = reinterpret_cast<void*>(getAddr);

    Dl_info info;
    dladdr(p, &info);

    m_fileName = info.dli_fname;
#else
    // nothing smart to do yet
#endif

    if (!initImGui())
    {
        cerr << "Couldn't initialize ImGui\n";
        return false;
    }

    TextureManager::createInstance();
    GlobalAssets::createInstance();
    ScriptManager::createInstance();

    setNumTasksPerUpdate(1);

    ////////////////////////////////////////////////////////////////
    // other init
    //glClearColor(0.0f, 0.1f, 0.4f, 1);
    glClearColor(50.f/255, 120.f/255, 185.f/255, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    setState(new LoadAllAssetsState);

    startRunning();

    return true;
}

void Application::deinitialize()
{
    ScriptManager::destroyInstance();
    TextureManager::destroyInstance();
    GlobalAssets::destroyInstance();
    deinitImGui();
    glfwDestroyWindow(m_window);
    m_window = nullptr;
    glfwTerminate();
}

void Application::beginFrame()
{
    m_currentState->beginFrame();
}

void Application::onKeyboardEvent(int key, int action, int mods)
{
    if (imguiHandleKeyboardEvent(key, action, mods)) return;
    if (m_currentState->onKeyboardEvent(key, action, mods)) return;


    if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(m_window, 1);
        }
#if !defined (__EMSCRIPTEN__)
        else if (key == GLFW_KEY_GRAVE_ACCENT)
        {
            m_isWireframe = !m_isWireframe;
            glPolygonMode(GL_FRONT_AND_BACK, m_isWireframe ? GL_LINE : GL_FILL);
        }
#endif
    }
}

void Application::onCharacterEvent(unsigned int codepoint)
{
    if (imguiHandleCharacterEvent(codepoint)) return;
    if (m_currentState->onCharacterEvent(codepoint)) return;
}

void Application::onMouseButtonEvent(int button, int action, int mods)
{
    if (imguiHandleMouseButtonEvent(button, action, mods)) return;
    if (m_currentState->onMouseButtonEvent(button, action, mods)) return;
}

void Application::onScrollEvent(double x, double y)
{
    if (imguiHandleScrollEvent(x, y)) return;
    if (m_currentState->onScrollEvent(x, y)) return;
}

void Application::handleInput()
{
    glfwPollEvents();

    if (glfwWindowShouldClose(m_window))
    {
        m_isRunning = false;
        return;
    }
}

void Application::update()
{
    TaskManager::update();
    updateImGui();

    m_currentState->update(m_timeSinceLastFrame);

    //static bool b = true;
    //ImGui::ShowDemoWindow(&b);
}

void Application::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_currentState->render();

    renderImGui();

    glfwSwapBuffers(m_window);
}

void Application::endFrame()
{
    m_currentState->endFrame();
}

void Application::mainLoop()
{
    uint32_t now = getTicks();
    m_timeSinceLastFrame = now - m_currentFrameTime;
    m_currentFrameTime = now;

    beginFrame();
    handleInput();
    update();
    render();
    endFrame();

    ++m_totalFrames;
    updateFPSData();

    checkForStateChange();

#if !defined (__EMSCRIPTEN__)
    uint32_t frameLength = getTicks() - m_currentFrameTime;
    if (frameLength < m_desiredFrameTime)
    {
        this_thread::sleep_for(chrono::milliseconds(m_desiredFrameTime - frameLength));
    }
#endif
}

void Application::updateFPSData()
{
    const uint32_t now = getTicks();
    const uint32_t fpsStatusUpdateTimeDelta = now - m_lastFPSStatusUpdateTime;

    if (fpsStatusUpdateTimeDelta > 1000) // update once per second
    {
        const uint32_t framesDelta = m_totalFrames - m_lastFPSStatusUpdateFrame;
        m_meanFrameTime = fpsStatusUpdateTimeDelta / framesDelta;
        m_fps = (framesDelta * 1000) / fpsStatusUpdateTimeDelta;

        m_lastFPSStatusUpdateTime = now;
        m_lastFPSStatusUpdateFrame = m_totalFrames;

        ostringstream sout;
        sout << "ft: " << m_meanFrameTime << " ms | fps: " << m_fps;
        glfwSetWindowTitle(m_window, sout.str().c_str());
    }
}

uint32_t Application::getTicks() const
{
    // actually the time returned is since the first time this function is called
    // but this is very early in the execution time, so it's fine

    static auto start = std::chrono::steady_clock::now();
    auto time = std::chrono::steady_clock::now() - start;
    return uint32_t(chrono::duration_cast<chrono::milliseconds>(time).count()) + 1;
}

void Application::destroyStates()
{
    m_currentState->deinitialize();
    delete m_currentState;
    m_currentState = nullptr;
    m_nextState = nullptr;
}

void Application::setState(AppState* newState)
{
    if (m_currentState)
    {
        // if there is an existing state finish its cycle, and then
        // set the new state on the next update
        if (m_nextState)
        {
            m_nextState->deinitialize();
            delete m_nextState;
        }

        m_nextState = newState;
    }
    else
    {
        assert(!m_nextState);

        if (!newState->initialize())
        {
            assert(false);
        }
        else
        {
            m_currentState = newState;
        }
    }
}

void Application::checkForStateChange()
{
    if (m_nextState)
    {
        // there is a next state to be set
        // finish with the current one and set the next

        assert(m_currentState);
        m_currentState->deinitialize();

        delete m_currentState;
        m_currentState = nullptr;

        if (!m_nextState->initialize())
        {
            assert(false);
        }
        else
        {
            m_currentState = m_nextState;
        }

        m_nextState = nullptr;
    }
}



namespace
{
Application* app = nullptr;
}

void Application::createInstance(int argc, char* argv[])
{
    app = new Application;
}

Application& Application::instance()
{
    return *app;
}

void Application::destroyInstance(int /*retCode*/)
{
    delete app;
    app = nullptr;
}
