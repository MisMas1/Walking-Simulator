#include "Player.hpp"
#include "Tools.hpp"

#include <iomanip>
#include <istream>
#include <ostream>

static void CopyItems(const std::vector<std::unique_ptr<Tool>> &from, std::vector<std::unique_ptr<Tool>> &to)
{
    to.clear();
    to.reserve(from.size());
    for (const std::unique_ptr<Tool> &item : from)
        to.push_back(item ? item->Clone() : nullptr);
}

static bool SameItems(const std::vector<std::unique_ptr<Tool>> &a, const std::vector<std::unique_ptr<Tool>> &b)
{
    if (a.size() != b.size())
        return false;

    for (std::size_t i = 0; i < a.size(); ++i)
    {
        if (!a[i] || !b[i])
        {
            if (a[i] != b[i])
                return false;
        }
        else if (*a[i] != *b[i])
        {
            return false;
        }
    }
    return true;
}

Player::Player() {}

Player::Player(const std::string &_name, const Vec3 &_position) : name(_name), position(_position) {}

Player::Player(const Player &other)
    : name(other.name), position(other.position), yaw(other.yaw), pitch(other.pitch),
      speed(other.speed), sprintSpeed(other.sprintSpeed), isSprinting(other.isSprinting),
      stamina(other.stamina), maxStamina(other.maxStamina), staminaDrain(other.staminaDrain),
      staminaRegen(other.staminaRegen), fear(other.fear), maxFear(other.maxFear),
      fearDecay(other.fearDecay), activeToolIndex(other.activeToolIndex), isAlive(other.isAlive)
{
    CopyItems(other.items, items);
}

Player &Player::operator=(const Player &other)
{
    if (this != &other)
    {
        name = other.name;
        position = other.position;
        yaw = other.yaw;
        pitch = other.pitch;
        speed = other.speed;
        sprintSpeed = other.sprintSpeed;
        isSprinting = other.isSprinting;
        stamina = other.stamina;
        maxStamina = other.maxStamina;
        staminaDrain = other.staminaDrain;
        staminaRegen = other.staminaRegen;
        fear = other.fear;
        maxFear = other.maxFear;
        fearDecay = other.fearDecay;
        CopyItems(other.items, items);
        activeToolIndex = other.activeToolIndex;
        isAlive = other.isAlive;
    }
    return *this;
}

bool Player::operator==(const Player &other) const
{
    return name == other.name && position == other.position && yaw == other.yaw &&
           pitch == other.pitch && speed == other.speed && sprintSpeed == other.sprintSpeed &&
           isSprinting == other.isSprinting && stamina == other.stamina &&
           maxStamina == other.maxStamina && staminaDrain == other.staminaDrain &&
           staminaRegen == other.staminaRegen && fear == other.fear && maxFear == other.maxFear &&
           fearDecay == other.fearDecay && SameItems(items, other.items) &&
           activeToolIndex == other.activeToolIndex && isAlive == other.isAlive;
}

bool Player::operator!=(const Player &other) const { return !(*this == other); }

void Player::Update(float deltaTime) {}
void Player::Move(const Vec3 &direction, float deltaTime) {}
void Player::Rotate(float deltaYaw, float deltaPitch) {}
void Player::AddFear(float amount) {}
Vec3 Player::GetPosition() const { return position; }

std::ostream &operator<<(std::ostream &os, const Player &p)
{
    std::size_t count = 0;
    for (const std::unique_ptr<Tool> &item : p.items)
        if (item)
            ++count;

    os << std::quoted(p.name) << ' ' << p.position << ' ' << p.yaw << ' ' << p.pitch << ' '
       << p.speed << ' ' << p.sprintSpeed << ' ' << p.isSprinting << ' ' << p.stamina << ' '
       << p.maxStamina << ' ' << p.staminaDrain << ' ' << p.staminaRegen << ' ' << p.fear << ' '
       << p.maxFear << ' ' << p.fearDecay << ' ' << p.activeToolIndex << ' ' << p.isAlive << ' '
       << count;

    for (const std::unique_ptr<Tool> &item : p.items)
        if (item)
            os << ' ' << *item;

    return os;
}

std::istream &operator>>(std::istream &is, Player &p)
{
    std::size_t count = 0;

    is >> std::quoted(p.name) >> p.position >> p.yaw >> p.pitch >> p.speed >> p.sprintSpeed >>
        p.isSprinting >> p.stamina >> p.maxStamina >> p.staminaDrain >> p.staminaRegen >> p.fear >>
        p.maxFear >> p.fearDecay >> p.activeToolIndex >> p.isAlive >> count;

    p.items.clear();
    for (std::size_t i = 0; i < count && is; ++i)
    {
        std::unique_ptr<Tool> tool = ReadTool(is);
        if (!tool)
        {
            is.setstate(std::ios::failbit);
            break;
        }
        p.items.push_back(std::move(tool));
    }
    return is;
}