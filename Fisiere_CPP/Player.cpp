#include "Player.hpp"
#include "Tools.hpp"

#include <algorithm>
#include <iomanip>
#include <istream>
#include <iterator>
#include <ostream>
#include <stdexcept>

using Items = std::vector<std::unique_ptr<Tool>>;

// Copiaza inventarul cu std::transform: fiecare unealta se copiaza prin Clone(),
// ca doua Player sa nu imparta aceleasi obiecte.
static void CopyItems(const Items &from, Items &to)
{
    to.clear();
    to.reserve(from.size());
    std::transform(from.begin(), from.end(), std::back_inserter(to),
                   [](const std::unique_ptr<Tool> &item) -> std::unique_ptr<Tool>
                   { return item ? item->Clone() : nullptr; });
}

// Compara doua inventare cu std::equal; folosim *x == *y (Tool::operator==, virtual).
static bool SameItems(const Items &a, const Items &b)
{
    return a.size() == b.size() &&
           std::equal(a.begin(), a.end(), b.begin(),
                      [](const std::unique_ptr<Tool> &x, const std::unique_ptr<Tool> &y)
                      {
                          if (!x || !y)
                              return !x && !y;
                          return *x == *y;
                      });
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

void Player::Update(float deltaTime)
{
    // Frica scade in timp; stamina scade la sprint si se reface la odihna.
    fear = std::clamp(fear - fearDecay * deltaTime, 0.0f, maxFear);

    const float staminaChange = isSprinting ? -staminaDrain : staminaRegen;
    stamina = std::clamp(stamina + staminaChange * deltaTime, 0.0f, maxStamina);

    if (stamina <= 0.0f)
        isSprinting = false;
}

void Player::Move(const Vec3 &direction, float deltaTime)
{
    const float currentSpeed = isSprinting ? sprintSpeed : speed;
    position.x += direction.x * currentSpeed * deltaTime;
    position.y += direction.y * currentSpeed * deltaTime;
    position.z += direction.z * currentSpeed * deltaTime;
}

void Player::Rotate(float deltaYaw, float deltaPitch)
{
    yaw += deltaYaw;
    pitch = std::clamp(pitch + deltaPitch, -89.0f, 89.0f);
}

void Player::AddFear(float amount)
{
    fear = std::clamp(fear + amount, 0.0f, maxFear);
}

Vec3 Player::GetPosition() const { return position; }

void Player::AddTool(std::unique_ptr<Tool> tool)
{
    if (tool)
        items.push_back(std::move(tool));
}

// std::any_of: cautare care raspunde doar "exista sau nu".
bool Player::HasTool(ToolType type) const
{
    return std::any_of(items.begin(), items.end(),
                       [type](const std::unique_ptr<Tool> &item) { return item && item->type == type; });
}

// std::find_if: cautare care intoarce pozitia primului element potrivit.
Tool &Player::GetTool(ToolType type)
{
    Items::iterator it = std::find_if(items.begin(), items.end(),
                                      [type](const std::unique_ptr<Tool> &item)
                                      { return item && item->type == type; });
    if (it == items.end())
        throw std::out_of_range("Player: unealta nu exista in inventar: " + ToString(type));
    return **it;
}

const Tool &Player::GetTool(ToolType type) const
{
    Items::const_iterator it = std::find_if(items.begin(), items.end(),
                                            [type](const std::unique_ptr<Tool> &item)
                                            { return item && item->type == type; });
    if (it == items.end())
        throw std::out_of_range("Player: unealta nu exista in inventar: " + ToString(type));
    return **it;
}

Tool &Player::ActiveTool()
{
    if (activeToolIndex < 0 || static_cast<std::size_t>(activeToolIndex) >= items.size() ||
        !items[activeToolIndex])
        throw std::out_of_range("Player: nu exista unealta activa");
    return *items[activeToolIndex];
}

const Tool &Player::ActiveTool() const
{
    if (activeToolIndex < 0 || static_cast<std::size_t>(activeToolIndex) >= items.size() ||
        !items[activeToolIndex])
        throw std::out_of_range("Player: nu exista unealta activa");
    return *items[activeToolIndex];
}

void Player::NextTool()
{
    if (items.empty())
        return;
    activeToolIndex = (activeToolIndex + 1) % static_cast<int>(items.size());
}

void Player::PrevTool()
{
    if (items.empty())
        return;
    const int count = static_cast<int>(items.size());
    activeToolIndex = (activeToolIndex + count - 1) % count;
}

std::ostream &operator<<(std::ostream &os, const Player &p)
{
    const auto count = std::count_if(p.items.begin(), p.items.end(),
                                     [](const std::unique_ptr<Tool> &item) { return static_cast<bool>(item); });

    os << std::quoted(p.name) << ' ' << p.position << ' ' << p.yaw << ' ' << p.pitch << ' '
       << p.speed << ' ' << p.sprintSpeed << ' ' << p.isSprinting << ' ' << p.stamina << ' '
       << p.maxStamina << ' ' << p.staminaDrain << ' ' << p.staminaRegen << ' ' << p.fear << ' '
       << p.maxFear << ' ' << p.fearDecay << ' ' << p.activeToolIndex << ' ' << p.isAlive << ' '
       << count;

    std::for_each(p.items.begin(), p.items.end(),
                  [&os](const std::unique_ptr<Tool> &item)
                  {
                      if (item)
                          os << ' ' << *item;
                  });

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