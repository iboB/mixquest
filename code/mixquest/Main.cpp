//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Application.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>

static void BreakMainLoop()
{
    emscripten_cancel_main_loop();
}

#else

static bool RunMainLoop = false;
static void BreakMainLoop()
{
    RunMainLoop = false;
}

#endif

static void Exit(int exitCode)
{
    BreakMainLoop();
    Application& theApp = Application::instance();
    theApp.deinitialize();
    Application::destroyInstance(exitCode);
}

static void MainLoop()
{
    Application& theApp = Application::instance();

    if (theApp.isRunning())
    {
        theApp.mainLoop();
    }
    else
    {
        Exit(0);
    }
}

int main(int argc, char* argv[])
{
    Application::createInstance(argc, argv);

    Application& theApp = Application::instance();

    Application::CreationParameters cp;
    cp.mainWindowTitle = "MixQuest";
    cp.desiredFrameTimeMs = 16;
    if (!theApp.initialize(cp))
    {
        Exit(1);
        return 1;
    }

    // main loop
#if defined(__EMSCRIPTEN__)
    emscripten_set_main_loop(MainLoop, 0, 1);
#else
    RunMainLoop = theApp.isRunning();
    if (RunMainLoop)
    {
        while (RunMainLoop)
        {
            MainLoop();
        }
    }
    else
    {
        Exit(0);
    }
#endif

    // WARNING:
    // Do not be tempted to add code here
    // In Emscripten it will be executed before even starting the main loop

    // For finalization code use the function Exit in this file

    return 0;
}

