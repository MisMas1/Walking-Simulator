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
    MenuAction DrawMainMenu(bool canContinue) override;
    MenuAction DrawPauseMenu() override;
    void SetMouseCaptured(bool captured) override;

    ~RaylibRenderer() override;

private:
    static Vector2 CalculateScreenResolution();
    void DrawMenuBackground(const char *heading, const char *subtitle);
    MenuAction DrawButton(const char *label, Rectangle bounds, MenuAction action);
    void DrawGameHUD();
    void DrawDetectionOverlay();

    Camera3D camera{};
    RenderTexture2D sceneTarget{};
    Shader lightingShader{};
    int flashlightUniform = -1;
    int screenAspectUniform = -1;
    float fearAmount = 0.0f;
    float staminaAmount = 0.0f;
    float batteryAmount = 0.0f;
    std::string activeToolName;
    bool initialized = false;
    bool shaderLoaded = false;
    bool detectionOverlayVisible = false;
};
