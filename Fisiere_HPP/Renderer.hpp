#pragma once

#include <string>

#include "Types.hpp"

#include "IRenderer.hpp"
#include "Player.hpp"
#include "Location.hpp"

// Desenatorul concret al jocului; implementeaza interfata abstracta IRenderer.
struct Renderer : IRenderer
{

    Renderer();

    bool Init() override;
    void Clear() override;
    void Present() override;
    void DrawScene(const Location &location, const Player &player) override;
    void DrawHUD(const Player &player) override;

    int windowWidth = 1280;
    int windowHeight = 720;
    std::string windowTitle;
    bool fullscreen = false;

    float ambientDarkness = 1.0f;
    Color backgroundColor;

    bool flashlightOn = false;
    float flashlightRadius = 8.0f;

    float fearOverlay = 0.0f;
    float screenShake = 0.0f;

    std::string currentSubtitle;
    float subtitleTimer = 0.0f;

    bool showHUD = true;
    bool showFPS = false;
};