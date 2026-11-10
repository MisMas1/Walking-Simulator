#include "Event.hpp"

#include <iomanip>
#include <istream>
#include <ostream>

Event::Event() {}

Event::Event(int _id, EventType _type, int _locationId, const Vec3 &_triggerPosition, float _fearAmount)
    : id(_id), type(_type), locationId(_locationId), triggerPosition(_triggerPosition), fearAmount(_fearAmount) {}

Event::Event(const Event &other)
    : id(other.id), type(other.type), locationId(other.locationId),
      triggerPosition(other.triggerPosition), triggerRadius(other.triggerRadius),
      requiredSafeCorners(other.requiredSafeCorners), triggerChance(other.triggerChance),
      oneTimeOnly(other.oneTimeOnly), hasRequiredTool(other.hasRequiredTool),
      requiredTool(other.requiredTool), fearAmount(other.fearAmount), duration(other.duration),
      soundName(other.soundName), message(other.message), triggered(other.triggered) {}

Event &Event::operator=(const Event &other)
{
    if (this != &other)
    {
        id = other.id;
        type = other.type;
        locationId = other.locationId;
        triggerPosition = other.triggerPosition;
        triggerRadius = other.triggerRadius;
        requiredSafeCorners = other.requiredSafeCorners;
        triggerChance = other.triggerChance;
        oneTimeOnly = other.oneTimeOnly;
        hasRequiredTool = other.hasRequiredTool;
        requiredTool = other.requiredTool;
        fearAmount = other.fearAmount;
        duration = other.duration;
        soundName = other.soundName;
        message = other.message;
        triggered = other.triggered;
    }
    return *this;
}

bool Event::operator==(const Event &other) const
{
    return id == other.id && type == other.type && locationId == other.locationId &&
           triggerPosition == other.triggerPosition && triggerRadius == other.triggerRadius &&
           requiredSafeCorners == other.requiredSafeCorners && triggerChance == other.triggerChance &&
           oneTimeOnly == other.oneTimeOnly && hasRequiredTool == other.hasRequiredTool &&
           requiredTool == other.requiredTool && fearAmount == other.fearAmount &&
           duration == other.duration && soundName == other.soundName && message == other.message &&
           triggered == other.triggered;
}

bool Event::operator!=(const Event &other) const { return !(*this == other); }

bool Event::operator<(const Event &other) const { return id < other.id; }

// Evenimentul se declanseaza cand jucatorul e destul de aproape si a trecut
// suficiente colturi "linistite" (mecanica de false alarme din GDD).
bool Event::ShouldTrigger(const Vec3 &playerPosition, int safeCorners) const
{
    if (triggered && oneTimeOnly)
        return false;
    if (safeCorners < requiredSafeCorners)
        return false;
    return Distance(playerPosition, triggerPosition) <= triggerRadius;
}

void Event::Trigger() { triggered = true; }

std::ostream &operator<<(std::ostream &os, const Event &e)
{
    return os << e.id << ' ' << e.type << ' ' << e.locationId << ' ' << e.triggerPosition << ' '
              << e.triggerRadius << ' ' << e.requiredSafeCorners << ' ' << e.triggerChance << ' '
              << e.oneTimeOnly << ' ' << e.hasRequiredTool << ' ' << e.requiredTool << ' '
              << e.fearAmount << ' ' << e.duration << ' ' << std::quoted(e.soundName) << ' '
              << std::quoted(e.message) << ' ' << e.triggered;
}

std::istream &operator>>(std::istream &is, Event &e)
{
    return is >> e.id >> e.type >> e.locationId >> e.triggerPosition >> e.triggerRadius >>
           e.requiredSafeCorners >> e.triggerChance >> e.oneTimeOnly >> e.hasRequiredTool >>
           e.requiredTool >> e.fearAmount >> e.duration >> std::quoted(e.soundName) >>
           std::quoted(e.message) >> e.triggered;
}