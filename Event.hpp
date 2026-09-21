#pragma once

#include <string>

#include "Types.hpp"

struct Event {
    int       id         = 0;
    EventType type       = EventType::Ambient;
    int       locationId = 0;

    Vec3     triggerPosition;
    float    triggerRadius       = 2.0f;
    int      requiredSafeCorners = 0;
    float    triggerChance       = 1.0f;   
    bool     oneTimeOnly         = true;
    bool     hasRequiredTool     = false;
    ToolType requiredTool        = ToolType::Flashlight;

    float       fearAmount = 0.0f;
    float       duration   = 0.0f;
    std::string soundName;
    std::string message;

    bool triggered = false;
};