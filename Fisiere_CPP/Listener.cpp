#include "Listener.hpp"

#include "Raylib.hpp"

Listener::Listener() {}

void Listener::Poll()
{
    ResetFrame();
    if (!inputEnabled)
        return;

    moveForward = IsKeyDown(KEY_W);
    moveBack = IsKeyDown(KEY_S);
    moveLeft = IsKeyDown(KEY_A);
    moveRight = IsKeyDown(KEY_D);
    sprint = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    flashlightToggle = IsKeyPressed(KEY_F);
    useTool = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    interact = IsKeyPressed(KEY_E);
    prevTool = IsKeyPressed(KEY_Q);
    nextTool = IsKeyPressed(KEY_R);
    pause = IsKeyPressed(KEY_ESCAPE);
    quit = WindowShouldClose();

    mouseDeltaX = static_cast<float>(GetMouseDelta().x);
    mouseDeltaY = static_cast<float>(GetMouseDelta().y);

    const float wheel = GetMouseWheelMove();
    if (wheel > 0.0f)
        prevTool = true;
    else if (wheel < 0.0f)
        nextTool = true;

    if (IsKeyPressed(KEY_ONE))
        toolSelection = 0;
    else if (IsKeyPressed(KEY_TWO))
        toolSelection = 1;
    else if (IsKeyPressed(KEY_THREE))
        toolSelection = 2;
    else if (IsKeyPressed(KEY_FOUR))
        toolSelection = 3;
    else if (IsKeyPressed(KEY_FIVE))
        toolSelection = 4;
}

void Listener::ResetFrame()
{
    flashlightToggle = false;
    useTool = false;
    interact = false;
    nextTool = false;
    prevTool = false;
    pause = false;
    quit = false;
    toolSelection = -1;
    mouseDeltaX = 0.0f;
    mouseDeltaY = 0.0f;
}