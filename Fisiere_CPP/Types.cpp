#include "Types.hpp"

#include <istream>
#include <ostream>

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

bool Vec3::operator==(const Vec3 &other) const
{
    return x == other.x && y == other.y && z == other.z;
}

bool Vec3::operator!=(const Vec3 &other) const { return !(*this == other); }

bool Vec3::operator<(const Vec3 &other) const
{
    if (x != other.x)
        return x < other.x;
    if (y != other.y)
        return y < other.y;
    return z < other.z;
}

bool Vec3::operator>(const Vec3 &other) const { return other < *this; }
bool Vec3::operator<=(const Vec3 &other) const { return !(other < *this); }
bool Vec3::operator>=(const Vec3 &other) const { return !(*this < other); }

std::ostream &operator<<(std::ostream &os, const Vec3 &v)
{
    return os << v.x << ' ' << v.y << ' ' << v.z;
}

std::istream &operator>>(std::istream &is, Vec3 &v)
{
    return is >> v.x >> v.y >> v.z;
}

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
    return r == other.r && g == other.g && b == other.b && a == other.a;
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