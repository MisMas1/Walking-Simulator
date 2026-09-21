#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Tool.hpp"
#include "Types.hpp"


struct Player {
    std::string name;

    Vec3  position;
    float yaw         = 0.0f;      
    float pitch       = 0.0f;      
    float speed       = 3.0f;
    float sprintSpeed = 5.5f;
    bool  isSprinting = false;

    float stamina      = 100.0f;
    float maxStamina   = 100.0f;
    float staminaDrain = 20.0f;
    float staminaRegen = 10.0f;

    float fear      = 0.0f;
    float maxFear   = 100.0f;
    float fearDecay = 2.0f;

    std::vector<std::unique_ptr<Tool>> items;
    int activeToolIndex = 0;

    bool isAlive = true;
};