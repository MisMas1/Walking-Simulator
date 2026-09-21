#pragma once

#include <vector>

#include "Event.hpp"
#include "Listener.hpp"
#include "Location.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "Story.hpp"
#include "Types.hpp"


struct GameEngine {
    bool      running   = false;
    GameState state     = GameState::Menu;
    float     deltaTime = 0.0f;
    float     totalTime = 0.0f;
    int       targetFPS = 60;

    Player       player;
    Listener     listener;
    Renderer     renderer;
    StoryManager story;

    std::vector<Location> locations;
    std::vector<Event>    events;
    int                   currentLocationIndex = 0;

    int   safeCorners        = 0;       
    float timeSinceLastScare = 0.0f;
    int   currentChapter     = 0;
};