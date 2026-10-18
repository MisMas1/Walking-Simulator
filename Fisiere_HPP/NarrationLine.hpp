#pragma once

#include <iosfwd>
#include <string>

#include "Types.hpp"

struct NarrationLine
{
    NarrationLine();
    NarrationLine(const std::string &_text, float _duration, int _triggerLocationId,
                  const Vec3 &_triggerPosition, int _chapter);
    NarrationLine(const NarrationLine &other);
    NarrationLine &operator=(const NarrationLine &other);

    bool operator==(const NarrationLine &other) const;
    bool operator!=(const NarrationLine &other) const;
    bool operator<(const NarrationLine &other) const; // ordine dupa capitol

    std::string text;
    float duration = 3.0f;
    int triggerLocationId = 0;
    Vec3 triggerPosition;
    int chapter = 0;
    bool played = false;
};

std::ostream &operator<<(std::ostream &os, const NarrationLine &line);
std::istream &operator>>(std::istream &is, NarrationLine &line);