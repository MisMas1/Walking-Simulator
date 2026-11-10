#include "Types.hpp"

#include <cmath>
#include <istream>
#include <ostream>
#include <tuple>

// ---------- Vec3 ----------

Vec3::Vec3() {}

Vec3::Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vec3::Vec3(const Vec3 &other) : x(other.x), y(other.y), z(other.z) {}

Vec3 &Vec3::operator=(const Vec3 &other)
{
    if (this != &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    return *this;
}

// std::tie leaga campurile intr-un tuplu; tuplurile se compara lexicografic
// (ca std::lexicographical_compare), deci nu mai scriem comparatia camp cu camp.
bool Vec3::operator==(const Vec3 &other) const
{
    return std::tie(x, y, z) == std::tie(other.x, other.y, other.z);
}

bool Vec3::operator!=(const Vec3 &other) const { return !(*this == other); }

bool Vec3::operator<(const Vec3 &other) const
{
    return std::tie(x, y, z) < std::tie(other.x, other.y, other.z);
}

bool Vec3::operator>(const Vec3 &other) const { return other < *this; }
bool Vec3::operator<=(const Vec3 &other) const { return !(other < *this); }
bool Vec3::operator>=(const Vec3 &other) const { return !(*this < other); }

float Distance(const Vec3 &a, const Vec3 &b)
{
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    const float dz = a.z - b.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

std::ostream &operator<<(std::ostream &os, const Vec3 &v)
{
    return os << v.x << ' ' << v.y << ' ' << v.z;
}

std::istream &operator>>(std::istream &is, Vec3 &v)
{
    return is >> v.x >> v.y >> v.z;
}

// ---------- Color ----------

Color::Color() {}

Color::Color(int _r, int _g, int _b, int _a) : r(_r), g(_g), b(_b), a(_a) {}

Color::Color(const Color &other) : r(other.r), g(other.g), b(other.b), a(other.a) {}

Color &Color::operator=(const Color &other)
{
    if (this != &other)
    {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
    }
    return *this;
}

bool Color::operator==(const Color &other) const
{
    return std::tie(r, g, b, a) == std::tie(other.r, other.g, other.b, other.a);
}

bool Color::operator!=(const Color &other) const { return !(*this == other); }

std::ostream &operator<<(std::ostream &os, const Color &c)
{
    return os << c.r << ' ' << c.g << ' ' << c.b << ' ' << c.a;
}

std::istream &operator>>(std::istream &is, Color &c)
{
    return is >> c.r >> c.g >> c.b >> c.a;
}

// ---------- Enum-uri ----------

std::string ToString(GameState state)
{
    switch (state)
    {
    case GameState::Menu:
        return "Menu";
    case GameState::Playing:
        return "Playing";
    case GameState::Paused:
        return "Paused";
    case GameState::Cutscene:
        return "Cutscene";
    case GameState::GameOver:
        return "GameOver";
    }
    return "Unknown";
}

std::string ToString(ToolType type)
{
    switch (type)
    {
    case ToolType::Flashlight:
        return "Flashlight";
    case ToolType::PolaroidCamera:
        return "Polaroid Camera";
    case ToolType::EMFMeter:
        return "EMF Meter";
    case ToolType::SpiritPod:
        return "Spirit Pod";
    case ToolType::DowsingRods:
        return "Dowsing Rods";
    }
    return "Unknown";
}

std::string ToString(EventType type)
{
    switch (type)
    {
    case EventType::Scripted:
        return "Scripted";
    case EventType::FalseAlarm:
        return "FalseAlarm";
    case EventType::Ambient:
        return "Ambient";
    case EventType::Narration:
        return "Narration";
    }
    return "Unknown";
}

std::ostream &operator<<(std::ostream &os, ToolType type)
{
    return os << static_cast<int>(type);
}

std::istream &operator>>(std::istream &is, ToolType &type)
{
    int value = 0;
    if (!(is >> value))
        return is;

    if (value < static_cast<int>(ToolType::Flashlight) ||
        value > static_cast<int>(ToolType::DowsingRods))
    {
        is.setstate(std::ios::failbit);
        return is;
    }

    type = static_cast<ToolType>(value);
    return is;
}

std::ostream &operator<<(std::ostream &os, EventType type)
{
    return os << static_cast<int>(type);
}

std::istream &operator>>(std::istream &is, EventType &type)
{
    int value = 0;
    if (!(is >> value))
        return is;

    if (value < static_cast<int>(EventType::Scripted) ||
        value > static_cast<int>(EventType::Narration))
    {
        is.setstate(std::ios::failbit);
        return is;
    }

    type = static_cast<EventType>(value);
    return is;
}