#include "Renderer.hpp"
#include "Location.hpp"
#include "Player.hpp"

Renderer::Renderer() {}
bool Renderer::Init() { return true;}
void Renderer::Clear() {}
void Renderer::Present() {}
void Renderer::DrawScene(const Location& location, const Player& player) {}
void Renderer::DrawHUD(const Player& player) {}