#include "NarrationLine.hpp"

#include <iomanip>
#include <istream>
#include <ostream>

NarrationLine::NarrationLine() {}

NarrationLine::NarrationLine(const std::string &_text, float _duration, int _triggerLocationId,
                             const Vec3 &_triggerPosition, int _chapter)
    : text(_text), duration(_duration), triggerLocationId(_triggerLocationId),
      triggerPosition(_triggerPosition), chapter(_chapter) {}

NarrationLine::NarrationLine(const NarrationLine &other)
    : text(other.text), duration(other.duration), triggerLocationId(other.triggerLocationId),
      triggerPosition(other.triggerPosition), chapter(other.chapter), played(other.played) {}

NarrationLine &NarrationLine::operator=(const NarrationLine &other)
{
    if (this != &other)
    {
        text = other.text;
        duration = other.duration;
        triggerLocationId = other.triggerLocationId;
        triggerPosition = other.triggerPosition;
        chapter = other.chapter;
        played = other.played;
    }
    return *this;
}

bool NarrationLine::operator==(const NarrationLine &other) const
{
    return text == other.text && duration == other.duration &&
           triggerLocationId == other.triggerLocationId &&
           triggerPosition == other.triggerPosition && chapter == other.chapter &&
           played == other.played;
}

bool NarrationLine::operator!=(const NarrationLine &other) const { return !(*this == other); }

bool NarrationLine::operator<(const NarrationLine &other) const { return chapter < other.chapter; }

std::ostream &operator<<(std::ostream &os, const NarrationLine &line)
{
    return os << std::quoted(line.text) << ' ' << line.duration << ' ' << line.triggerLocationId
              << ' ' << line.triggerPosition << ' ' << line.chapter << ' ' << line.played;
}

std::istream &operator>>(std::istream &is, NarrationLine &line)
{
    return is >> std::quoted(line.text) >> line.duration >> line.triggerLocationId >>
           line.triggerPosition >> line.chapter >> line.played;
}