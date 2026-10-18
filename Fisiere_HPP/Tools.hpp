#pragma once

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include "Tool.hpp"
#include "Types.hpp"

struct Flashlight : Tool
{
    Flashlight();
    Flashlight(float _radius, float _intensity);
    Flashlight(const Flashlight &other);
    Flashlight &operator=(const Flashlight &other);

    void Use() override;
    void Update(float deltaTime) override;
    std::unique_ptr<Tool> Clone() const override;
    bool Equals(const Tool &other) const override;
    void Print(std::ostream &os) const override;
    void Read(std::istream &is) override;

    float radius = 8.0f;
    float intensity = 1.0f;
    float flickerChance = 0.0f;
    bool isFlickering = false;
};

struct Photo
{
    Photo();
    Photo(const Vec3 &_position, bool _anomalyCaptured, const std::string &_description);
    Photo(const Photo &other);
    Photo &operator=(const Photo &other);

    bool operator==(const Photo &other) const;
    bool operator!=(const Photo &other) const;

    Vec3 position;
    bool anomalyCaptured = false;
    std::string description;
};

std::ostream &operator<<(std::ostream &os, const Photo &photo);
std::istream &operator>>(std::istream &is, Photo &photo);

struct PolaroidCamera : Tool
{
    PolaroidCamera();
    PolaroidCamera(int _photosLeft, float _flashCooldown);
    PolaroidCamera(const PolaroidCamera &other);
    PolaroidCamera &operator=(const PolaroidCamera &other);

    void Use() override;
    void Update(float deltaTime) override;
    std::unique_ptr<Tool> Clone() const override;
    bool Equals(const Tool &other) const override;
    void Print(std::ostream &os) const override;
    void Read(std::istream &is) override;

    int photosLeft = 10;
    std::vector<Photo> photos;
    float flashCooldown = 2.0f;
};

struct EMFMeter : Tool
{
    EMFMeter();
    EMFMeter(float _detectionRange, float _sensitivity);
    EMFMeter(const EMFMeter &other);
    EMFMeter &operator=(const EMFMeter &other);

    void Use() override;
    void Update(float deltaTime) override;
    std::unique_ptr<Tool> Clone() const override;
    bool Equals(const Tool &other) const override;
    void Print(std::ostream &os) const override;
    void Read(std::istream &is) override;

    int currentLevel = 1; 
    float detectionRange = 10.0f;
    float sensitivity = 1.0f;
};

struct SpiritPod : Tool
{
    SpiritPod();
    SpiritPod(float _responseChance, float _range);
    SpiritPod(const SpiritPod &other);
    SpiritPod &operator=(const SpiritPod &other);

    void Use() override;
    void Update(float deltaTime) override;
    std::unique_ptr<Tool> Clone() const override;
    bool Equals(const Tool &other) const override;
    void Print(std::ostream &os) const override;
    void Read(std::istream &is) override;

    bool isListening = false;
    float responseChance = 0.1f;
    std::string lastResponse;
    float range = 8.0f;
};

struct DowsingRods : Tool
{
    DowsingRods();
    DowsingRods(float _detectionRange);
    DowsingRods(const DowsingRods &other);
    DowsingRods &operator=(const DowsingRods &other);

    void Use() override;
    void Update(float deltaTime) override;
    std::unique_ptr<Tool> Clone() const override;
    bool Equals(const Tool &other) const override;
    void Print(std::ostream &os) const override;
    void Read(std::istream &is) override;

    float angle = 0.0f;
    Vec3 pointingAt;
    float detectionRange = 10.0f;
};

std::unique_ptr<Tool> MakeTool(ToolType type);
std::unique_ptr<Tool> ReadTool(std::istream &is);