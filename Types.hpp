#pragma once


struct Vec3 {
    float x = 0.0f;   
    float y = 0.0f;  
    float z = 0.0f;   
};

struct Color {
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