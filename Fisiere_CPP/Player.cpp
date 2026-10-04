#include "Player.hpp"

Player::Player() {}
void Player::Update(float deltaTime) {}
void Player::Move(const Vec3& direction, float deltaTime) {}
void Player::Rotate(float deltaYaw, float deltaPitch) {}
void Player::AddFear(float amount) {}
Vec3 Player::GetPosition() const { return position;}