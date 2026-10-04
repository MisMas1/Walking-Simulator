#pragma once

#include <map>
#include <string>
#include <vector>

#include "NarrationLine.hpp"

struct StoryManager
{
    StoryManager();

    void Update(float deltaTime);
    void AddLine(const NarrationLine &line);

    std::vector<NarrationLine> lines;
    int currentLineIndex = 0;
    int currentChapter = 0;
    bool isNarrating = false;
    float narrationTimer = 0.0f;

    std::vector<std::string> loreEntries;
    std::map<std::string, bool> endingFlags;
};