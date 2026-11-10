#pragma once

#include "Raylib.hpp"
#include "Renderer.hpp"

struct RaylibRenderer : Renderer
{
    bool Init() override;
    void Clear() override;
    void Present() override;
    void DrawScene(const Location &location, const Player &player) override;
    void DrawHUD(const Player &player) override;

    ~RaylibRenderer() override;

private:
    Camera3D camera{};
    bool initialized = false;
};
