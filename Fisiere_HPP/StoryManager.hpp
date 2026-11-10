#pragma once

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "NarrationLine.hpp"

struct StoryManager
{
    StoryManager();

    void Update(float deltaTime);

    // Inserarea pastreaza lista sortata dupa capitol (std::upper_bound).
    void AddLine(const NarrationLine &line);

    // Cauta prima replica nejucata din locatia data (std::find_if) si o marcheaza ca jucata.
    // Intoarce textul ei, sau std::nullopt daca nu mai e nicio replica (fara pointeri).
    std::optional<std::string> PlayNext(int locationId);

    std::size_t RemainingLines() const;

    void AddLore(const std::string &entry);
    bool HasLore(const std::string &entry) const;

    std::vector<NarrationLine> lines;
    int currentLineIndex = 0;
    int currentChapter = 0;
    bool isNarrating = false;
    float narrationTimer = 0.0f;

    std::vector<std::string> loreEntries;
    std::map<std::string, bool> endingFlags;
};