#include "Tool.hpp"

#include <iomanip>
#include <istream>
#include <ostream>

Tool::Tool() {}

Tool::Tool(ToolType _type, const std::string &_name, float _maxBattery, float _batteryDrain)
    : type(_type), name(_name), battery(_maxBattery), maxBattery(_maxBattery), batteryDrain(_batteryDrain) {}

Tool::Tool(const Tool &other)
    : type(other.type), name(other.name), isActive(other.isActive), battery(other.battery),
      maxBattery(other.maxBattery), batteryDrain(other.batteryDrain), cooldown(other.cooldown),
      cooldownTimer(other.cooldownTimer), requiresBattery(other.requiresBattery) {}

Tool::~Tool() {}

Tool &Tool::operator=(const Tool &other)
{
    if (this != &other)
    {
        type = other.type;
        name = other.name;
        isActive = other.isActive;
        battery = other.battery;
        maxBattery = other.maxBattery;
        batteryDrain = other.batteryDrain;
        cooldown = other.cooldown;
        cooldownTimer = other.cooldownTimer;
        requiresBattery = other.requiresBattery;
    }
    return *this;
}

void Tool::Use() {}

bool Tool::IsReady() const { return true; }

bool Tool::Equals(const Tool &other) const
{
    return type == other.type && name == other.name && isActive == other.isActive &&
           battery == other.battery && maxBattery == other.maxBattery &&
           batteryDrain == other.batteryDrain && cooldown == other.cooldown &&
           cooldownTimer == other.cooldownTimer && requiresBattery == other.requiresBattery;
}

bool Tool::operator==(const Tool &other) const { return Equals(other); }

bool Tool::operator!=(const Tool &other) const { return !Equals(other); }

void Tool::Print(std::ostream &os) const
{
    os << std::quoted(name) << ' ' << isActive << ' ' << battery << ' ' << maxBattery << ' '
       << batteryDrain << ' ' << cooldown << ' ' << cooldownTimer << ' ' << requiresBattery;
}

void Tool::Read(std::istream &is)
{
    is >> std::quoted(name) >> isActive >> battery >> maxBattery >> batteryDrain >> cooldown >>
        cooldownTimer >> requiresBattery;
}

std::ostream &operator<<(std::ostream &os, const Tool &tool)
{
    os << tool.type << ' ';
    tool.Print(os);
    return os;
}

std::istream &operator>>(std::istream &is, Tool &tool)
{
    ToolType type;
    if (!(is >> type))
        return is;

    if (type != tool.type)
    {
        is.setstate(std::ios::failbit);
        return is;
    }

    tool.Read(is);
    return is;
}