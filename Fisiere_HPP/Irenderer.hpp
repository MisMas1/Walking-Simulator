#pragma once

struct Player;
struct Location;

struct IRenderer
{
    virtual ~IRenderer();

    virtual bool Init() = 0;
    virtual void Clear() = 0;
    virtual void Present() = 0;
    virtual void DrawScene(const Location &location, const Player &player) = 0;
    virtual void DrawHUD(const Player &player) = 0;
};