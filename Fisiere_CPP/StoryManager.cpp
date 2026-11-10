#include "StoryManager.hpp"

#include <algorithm>
#include <iterator>

StoryManager::StoryManager() {}

void StoryManager::Update(float deltaTime)
{
    if (!isNarrating)
        return;

    narrationTimer -= deltaTime;
    if (narrationTimer <= 0.0f)
    {
        narrationTimer = 0.0f;
        isNarrating = false;
    }
}

void StoryManager::AddLine(const NarrationLine &line)
{
    // upper_bound foloseste NarrationLine::operator< (dupa capitol) si gaseste
    // pozitia unde linia nu strica ordinea; replicile cu acelasi capitol raman in ordinea adaugarii.
    std::vector<NarrationLine>::iterator position = std::upper_bound(lines.begin(), lines.end(), line);
    lines.insert(position, line);
}

std::optional<std::string> StoryManager::PlayNext(int locationId)
{
    std::vector<NarrationLine>::iterator it =
        std::find_if(lines.begin(), lines.end(),
                     [locationId](const NarrationLine &l) { return !l.played && l.triggerLocationId == locationId; });

    if (it == lines.end())
        return std::nullopt;

    it->played = true;
    isNarrating = true;
    narrationTimer = it->duration;
    currentLineIndex = static_cast<int>(std::distance(lines.begin(), it));
    currentChapter = it->chapter;
    return it->text;
}

std::size_t StoryManager::RemainingLines() const
{
    return static_cast<std::size_t>(
        std::count_if(lines.begin(), lines.end(), [](const NarrationLine &l) { return !l.played; }));
}

void StoryManager::AddLore(const std::string &entry)
{
    if (!HasLore(entry))
        loreEntries.push_back(entry);
}

bool StoryManager::HasLore(const std::string &entry) const
{
    return std::find(loreEntries.begin(), loreEntries.end(), entry) != loreEntries.end();
}