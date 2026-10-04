#pragma once

#include <string>
#include <vector>

#include "Tool.hpp"
#include "Types.hpp"

struct Flashlight : Tool
{
    Flashlight();
    void Use() override;
    void Update(float deltaTime) override;

    float radius = 8.0f;
    float intensity = 1.0f;
    float flickerChance = 0.0f;
    bool isFlickering = false;
};

struct Photo
{
    Photo();

    Vec3 position;
    bool anomalyCaptured = false;
    std::string description;
};

struct PolaroidCamera : Tool
{
    PolaroidCamera();
    void Use() override;
    void Update(float deltaTime) override;

    int photosLeft = 10;
    std::vector<Photo> photos;
    float flashCooldown = 2.0f;
};

struct EMFMeter : Tool
{
    EMFMeter();
    void Use() override;
    void Update(float deltaTime) override;

    int currentLevel = 1; // 1 - 5
    float detectionRange = 10.0f;
    float sensitivity = 1.0f;
};

struct SpiritPod : Tool
{
    SpiritPod();
    void Use() override;
    void Update(float deltaTime) override;

    bool isListening = false;
    float responseChance = 0.1f;
    std::string lastResponse;
    float range = 8.0f;
};

struct DowsingRods : Tool
{
    DowsingRods();
    void Use() override;
    void Update(float deltaTime) override;

    float angle = 0.0f;
    Vec3 pointingAt;
    float detectionRange = 10.0f;
};