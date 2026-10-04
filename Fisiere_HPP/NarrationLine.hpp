#pragma once

#include <string>

#include "Types.hpp"

struct NarrationLine
{
    NarrationLine();

    std::string text;
    float duration = 3.0f;
    int triggerLocationId = 0;
    Vec3 triggerPosition;
    int chapter = 0;
    bool played = false;
};