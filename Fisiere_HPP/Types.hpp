#pragma once

#include <iosfwd>

struct Vec3
{
    Vec3();
    Vec3(float _x, float _y, float _z);
    Vec3(const Vec3 &other);
    Vec3 &operator=(const Vec3 &other);

    bool operator==(const Vec3 &other) const;
    bool operator!=(const Vec3 &other) const;
    bool operator<(const Vec3 &other) const;
    bool operator>(const Vec3 &other) const;
    bool operator<=(const Vec3 &other) const;
    bool operator>=(const Vec3 &other) const;

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct Color
{
    Color();
    Color(int _r, int _g, int _b, int _a = 255);
    Color(const Color &other);
    Color &operator=(const Color &other);

    bool operator==(const Color &other) const;
    bool operator!=(const Color &other) const;

    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;
};

enum class GameState {
    Menu,
    Playing,
    Paused,
    Cutscene,
    GameOver
};

enum class Action {
    None,
    MoveForward,
    MoveBack,
    MoveLeft,
    MoveRight,
    Sprint,
    ToggleFlashlight,
    UseTool,
    Interact,
    NextTool,
    PrevTool,
    Pause,
    Quit
};

enum class ToolType {
    Flashlight,
    PolaroidCamera,
    EMFMeter,
    SpiritPod,
    DowsingRods
};

enum class EventType {
    Scripted,
    FalseAlarm,
    Ambient,
    Narration
};

// Operatori de intrare / iesire.
// Format text: valorile sunt separate prin spatiu, enum-urile se scriu ca numar.
std::ostream &operator<<(std::ostream &os, const Vec3 &v);
std::istream &operator>>(std::istream &is, Vec3 &v);

std::ostream &operator<<(std::ostream &os, const Color &c);
std::istream &operator>>(std::istream &is, Color &c);

std::ostream &operator<<(std::ostream &os, ToolType type);
std::istream &operator>>(std::istream &is, ToolType &type);

std::ostream &operator<<(std::ostream &os, EventType type);
std::istream &operator>>(std::istream &is, EventType &type);