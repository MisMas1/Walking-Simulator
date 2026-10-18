#pragma once

struct Player;
struct Location;

// Clasa de baza abstracta pentru redarea obiectelor de joc.
// Toate metodele sunt pur virtuale, deci nu se poate crea un obiect IRenderer direct.
// Orice desenator (Renderer, un viitor desenator in consola sau in OpenGL) o mosteneste.
struct IRenderer
{
    virtual ~IRenderer();

    virtual bool Init() = 0;
    virtual void Clear() = 0;
    virtual void Present() = 0;
    virtual void DrawScene(const Location &location, const Player &player) = 0;
    virtual void DrawHUD(const Player &player) = 0;
};