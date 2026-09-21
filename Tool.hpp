#pragma once

#include <string>

#include "Types.hpp"


struct Tool {
    ToolType    type;
    std::string name;

    bool  isActive        = false;
    float battery         = 100.0f;
    float maxBattery      = 100.0f;
    float batteryDrain    = 0.0f;    
    float cooldown        = 0.0f;    
    float cooldownTimer   = 0.0f;    
    bool  requiresBattery = true;
};