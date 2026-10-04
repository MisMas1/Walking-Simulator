#pragma once

#include <string>

#include "Types.hpp"

#include "Player.hpp"
#include "Location.hpp"

struct Renderer
{

    Renderer();

    bool Init();
    void Clear();
    void Present();
    void DrawScene(const Location &location, const Player &player);
    void DrawHUD(const Player &player);

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