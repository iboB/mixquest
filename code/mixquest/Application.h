//
// MixQuest
// Copyright (c) 2017-2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "TaskManager.h"

struct GLFWwindow;
class ImGuiRenderData;
class AppState;

class Application : public TaskManager
{
private:
    Application();
    ~Application();

public:
    static void createInstance(int argc, char* argv[]);
    static Application& instance();
    static void destroyInstance(int retCode);

    bool isRunning() const { return m_isRunning; }

    struct CreationParameters
    {
        CreationParameters();
        // desired frame time in milliseconds
        // ignored in emscripten (frame-rate is fixed by the browser)
        // 0 means the fastest possible framerate
        uint32_t desiredFrameTimeMs = 0;

        const char* mainWindowTitle = "Demo App";
        yama::uvector2 mainWindowClientAreaSize = yama::vt(800u, 600u);
        bool isFullScreen = false;
    };

    // return false on error
    bool initialize(CreationParameters creationParameters = {});
    void deinitialize();

    // Function to be called for each appliacation frame
    void mainLoop();

    // name of application file on desktop platforms
    const std::string& fileName() const { return m_fileName; }

private:

    void startRunning() { m_isRunning = true; }
    void stopRunning() { m_isRunning = false; }

    void beginFrame();
    void handleInput();
    void update();
    void render();
    void endFrame();

    GLFWwindow* m_window = nullptr;

private:
    bool m_isRunning = false;

    // wireframe rendering
    // not supported on WebGL or GL ES platforms
    bool m_isWireframe = false;

    // filename of the application with full path
    std::string m_fileName;

    //////////////////////////////////////
    // input
public:
    void onKeyboardEvent(int key, int action, int mods);
    void onCharacterEvent(unsigned int codepoint);
    void onMouseButtonEvent(int button, int action, int mods);
    void onScrollEvent(double x, double y);

    //////////////////////////////////////
    // ImGui
private:
    bool initImGui();
    void updateImGui();
    void deinitImGui();
    void renderImGui();

    bool imguiHandleKeyboardEvent(int key, int action, int mods);
    bool imguiHandleCharacterEvent(unsigned int codepoint);
    bool imguiHandleMouseButtonEvent(int button, int action, int mods);
    bool imguiHandleScrollEvent(double x, double y);

    ImGuiRenderData* m_imguiRenderData;

    //////////////////////////////////////
    // time and fps stuff
public:

    // time in ms since the application has started
    uint32_t getTicks() const;

    // total number of frames the aplication has run
    uint32_t totalFrames() const { return m_totalFrames; }
private:
    uint32_t m_currentFrameTime = 0; // start of current frame (ms)
    uint32_t m_timeSinceLastFrame = 0; // used as delta for updates

    uint32_t m_desiredFrameTime = 0; // the application will try to work at this time per frame

    void updateFPSData();

    uint32_t m_totalFrames = 0; // number of frames since the app has started
    uint32_t m_lastFrameEnd = 0; // end time of the last frame
    uint32_t m_lastFPSStatusUpdateTime = 0; // last time the fps status was updated
    uint32_t m_lastFPSStatusUpdateFrame = 0; // frame number of the last time the status was updated

    // results
    uint32_t m_meanFrameTime;
    uint32_t m_fps;

    //////////////////////////////////////
    // states
public:
    void setState(AppState* state);

    // forcefully destroys current and pending states
    // if you call this yourself, be careful to set a valid state afterwards
    void destroyStates();

private:
    // check if there is a state pending to be set
    void checkForStateChange();

    AppState* m_currentState = nullptr;
    AppState* m_nextState = nullptr;
};
