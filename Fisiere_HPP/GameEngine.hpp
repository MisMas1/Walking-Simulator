#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Event.hpp"
#include "Listener.hpp"
#include "Location.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "StoryManager.hpp"
#include "NarrationLine.hpp"
#include "Types.hpp"

struct GameEngine
{

    GameEngine();

    void Init();
    void Run();
    void Shutdown();

    // Salvare / incarcare cu fluxuri de fisier (std::ofstream / std::ifstream).
    bool SaveGame(const std::string &path) const;
    bool LoadGame(const std::string &path);

    // Cautari: intorc referinte si arunca std::out_of_range daca elementul nu exista.
    Location &FindLocation(int id);
    Event &FindEvent(int id);
    std::vector<Event> EventsInLocation(int locationId) const;

    void ChangeLocation(int id);

    // Declanseaza evenimentele eligibile din locatia curenta si intoarce mesajele lor.
    std::vector<std::string> TriggerEvents();

    bool running = false;
    GameState state = GameState::Menu;
    float deltaTime = 0.0f;
    float totalTime = 0.0f;
    int targetFPS = 60;

    Player player;
    Listener listener;
    std::unique_ptr<Renderer> renderer; // smart pointer la clasa abstracta; obiectul real e ConsoleRenderer
    StoryManager story;

    std::map<int, Location> locations; // cheia = id-ul locatiei
    std::vector<Event> events;
    int currentLocationId = 0;

    int safeCorners = 0;
    float timeSinceLastScare = 0.0f;
    int currentChapter = 0;
};