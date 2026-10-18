#pragma once

#include <iosfwd>
#include <string>

#include "Types.hpp"

struct Event
{

    Event();
    Event(int _id, EventType _type, int _locationId, const Vec3 &_triggerPosition, float _fearAmount);
    Event(const Event &other);
    Event &operator=(const Event &other);

    bool operator==(const Event &other) const;
    bool operator!=(const Event &other) const;
    bool operator<(const Event &other) const; // ordine dupa id

    void Trigger();
    bool ShouldTrigger(const Vec3& playerPosition, int safeCorners) const;

    int id = 0;
    EventType type = EventType::Ambient;
    int locationId = 0;

    Vec3 triggerPosition;
    float triggerRadius = 2.0f;
    int requiredSafeCorners = 0;
    float triggerChance = 1.0f;
    bool oneTimeOnly = true;
    bool hasRequiredTool = false;
    ToolType requiredTool = ToolType::Flashlight;

    float fearAmount = 0.0f;
    float duration = 0.0f;
    std::string soundName;
    std::string message;

    bool triggered = false;
};

std::ostream &operator<<(std::ostream &os, const Event &event);
std::istream &operator>>(std::istream &is, Event &event);