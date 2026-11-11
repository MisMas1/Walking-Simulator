#pragma once

#include <string>

#include "Types.hpp"

#include "Player.hpp"
#include "Location.hpp"

enum class MenuAction
{
    None,
    Start,
    Continue,
    Resume,
    MainMenu,
    Quit
};

// Clasa de baza abstracta pentru redarea obiectelor de joc.
// Metodele de desenare sunt pur virtuale (= 0), deci nu se poate crea un obiect Renderer direct.
// Un desenator concret (de exemplu ConsoleRenderer) o mosteneste si implementeaza metodele.
struct Renderer
{

    Renderer();
    virtual ~Renderer(); // virtual: desenatorul se sterge prin pointer la Renderer

    virtual bool Init() = 0;
    virtual void Clear() = 0;
    virtual void Present() = 0;
    virtual void DrawScene(const Location &location, const Player &player) = 0;
    virtual void DrawHUD(const Player &player) = 0;
    virtual MenuAction DrawMainMenu(bool) { return MenuAction::None; }
    virtual MenuAction DrawPauseMenu() { return MenuAction::None; }
    virtual void SetMouseCaptured(bool) {}

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
    std::string currentLocationName;
    bool flashlightEnabled = false;
    float flashlightFlickerTimer = 0.0f;
    float cameraFlashTimer = 0.0f;
    bool ghostVisible = false;
    Vec3 ghostPosition;
    bool rodsPlaced = false;
    int rodsLocationId = -1;
    Vec3 rodsPosition;

    bool showHUD = true;
    bool showFPS = false;
};