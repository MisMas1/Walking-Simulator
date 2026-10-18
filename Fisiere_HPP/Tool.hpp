#pragma once

#include <iosfwd>
#include <memory>
#include <string>

#include "Types.hpp"

struct Tool
{

    Tool();
    Tool(ToolType _type, const std::string &_name, float _maxBattery = 100.0f, float _batteryDrain = 0.0f);
    Tool(const Tool &other);
    virtual ~Tool(); 
    Tool &operator=(const Tool &other);

    virtual void Use();
    virtual void Update(float deltaTime) = 0;
    bool IsReady() const;

    virtual std::unique_ptr<Tool> Clone() const = 0;

    virtual bool Equals(const Tool &other) const;
    bool operator==(const Tool &other) const;
    bool operator!=(const Tool &other) const;

    virtual void Print(std::ostream &os) const;
    virtual void Read(std::istream &is);

    ToolType type = ToolType::Flashlight;
    std::string name;

    bool isActive = false;
    float battery = 100.0f;
    float maxBattery = 100.0f;
    float batteryDrain = 0.0f;
    float cooldown = 0.0f;
    float cooldownTimer = 0.0f;
    bool requiresBattery = true;
};

std::ostream &operator<<(std::ostream &os, const Tool &tool);
std::istream &operator>>(std::istream &is, Tool &tool);