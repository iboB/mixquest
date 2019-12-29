//
// MixQuest
// Copyright (c) 2017-2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// base class for application states

class AppState
{
public:
    virtual ~AppState() {}

    // return true on successful initialization
    virtual bool initialize() = 0;
    virtual void deinitialize() = 0;

    // not pure virtual since you may want to create a state with no input
    // return value shows whether the event has been processed by this handler
    virtual bool onKeyboardEvent(int key, int action, int mods) { return false; };
    virtual bool onCharacterEvent(unsigned int codepoint) { return false; };
    virtual bool onMouseButtonEvent(int button, int action, int mods) { return false; };
    virtual bool onScrollEvent(double x, double y) { return false; };

    // called at the start of each frame before anything else
    virtual void beginFrame() = 0;

    // dt is time delta since last frame
    virtual void update(uint32_t dt) = 0;

    // do your rendering here
    virtual void render() = 0;

    // called at the end of each frame
    virtual void endFrame() = 0;
};
