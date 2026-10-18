#pragma once

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include "Tool.hpp"
#include "Types.hpp"

struct Player
{

    Player();
    Player(const std::string &_name, const Vec3 &_position);
    Player(const Player &other); 
    Player &operator=(const Player &other);

    bool operator==(const Player &other) const;
    bool operator!=(const Player &other) const;

    void Update(float deltaTime);
    void Move(const Vec3 &direction, float deltaTime);
    void Rotate(float deltaYaw, float deltaPitch);
    void AddFear(float amount);
    Vec3 GetPosition() const;

    std::string name;

    Vec3 position;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float speed = 3.0f;
    float sprintSpeed = 5.5f;
    bool isSprinting = false;

    float stamina = 100.0f;
    float maxStamina = 100.0f;
    float staminaDrain = 20.0f;
    float staminaRegen = 10.0f;

    float fear = 0.0f;
    float maxFear = 100.0f;
    float fearDecay = 2.0f;

    std::vector<std::unique_ptr<Tool>> items;
    int activeToolIndex = 0;

    bool isAlive = true;
};

std::ostream &operator<<(std::ostream &os, const Player &player);
std::istream &operator>>(std::istream &is, Player &player);