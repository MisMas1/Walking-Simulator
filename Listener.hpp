#pragma once

#include <map>

#include "Types.hpp"

struct Listener {
    bool moveForward = false;
    bool moveBack    = false;
    bool moveLeft    = false;
    bool moveRight   = false;
    bool sprint      = false;

    bool flashlightToggle = false;
    bool useTool          = false;
    bool interact         = false;
    bool nextTool         = false;
    bool prevTool         = false;
    bool pause            = false;
    bool quit             = false;

    float mouseDeltaX      = 0.0f;
    float mouseDeltaY      = 0.0f;
    float mouseSensitivity = 0.1f;

    std::map<int, Action> keyBindings;  
    bool inputEnabled = true;            
};