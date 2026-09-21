#pragma once

#include <map>
#include <string>
#include <vector>

#include "Types.hpp"


struct NarrationLine {
    std::string text;
    float       duration          = 3.0f;
    int         triggerLocationId = 0;
    Vec3        triggerPosition;
    int         chapter           = 0;
    bool        played            = false;
};

struct StoryManager {
    std::vector<NarrationLine> lines;
    int   currentLineIndex = 0;
    int   currentChapter   = 0;
    bool  isNarrating      = false;
    float narrationTimer   = 0.0f;

    std::vector<std::string>    loreEntries;
    std::map<std::string, bool> endingFlags;   
};