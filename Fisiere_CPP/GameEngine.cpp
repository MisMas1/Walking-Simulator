#include "GameEngine.hpp"
#include "ConsoleRenderer.hpp"
#include "Tools.hpp"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <set>
#include <stdexcept>

GameEngine::GameEngine() : renderer(std::make_unique<ConsoleRenderer>()) {}

// ---------- Functii ajutatoare (vizibile doar in acest fisier) ----------

static Location MakeLocation(int id, const std::string &name, const std::string &description,
                             const std::set<int> &exits, const std::set<int> &eventIds)
{
    Location location(id, name, description, true);
    location.exits = exits;
    location.events = eventIds;
    return location;
}

static Event MakeEvent(int id, EventType type, int locationId, float fear, int requiredSafeCorners,
                       const std::string &message)
{
    Event event(id, type, locationId, Vec3(), fear);
    event.requiredSafeCorners = requiredSafeCorners;
    event.message = message;
    return event;
}

// ---------- Ciclul de viata ----------

void GameEngine::Init()
{
    renderer->windowTitle = "Gonjiam: Ghost Hunt";
    if (!renderer->Init())
        return;

    player = Player("Dima", Vec3());
    player.AddTool(std::make_unique<Flashlight>());
    player.AddTool(std::make_unique<EMFMeter>());
    player.AddTool(std::make_unique<PolaroidCamera>());

    // std::map: locatiile se gasesc dupa id si raman ordonate dupa id.
    const std::vector<Location> built = {
        MakeLocation(0, "Padurea", "Un drum de pamant printre copaci, noaptea. In departare se zareste spitalul.", {1}, {}),
        MakeLocation(1, "Curtea spitalului", "O poarta ruginita si o curte plina de buruieni.", {0, 2}, {1}),
        MakeLocation(2, "Holul principal", "Un hol lung. Tencuiala se cojeste de pe pereti.", {1, 3}, {2, 3}),
        MakeLocation(3, "Subsolul", "Aer rece si miros de umezeala.", {2}, {4}),
    };
    locations.clear();
    std::for_each(built.begin(), built.end(), [this](const Location &l) { locations.emplace(l.id, l); });

    // Evenimentele sunt adaugate intentionat in alta ordine, apoi sortate dupa id (Event::operator<).
    events = {
        MakeEvent(4, EventType::Ambient, 3, 10.0f, 0, "Se aude o respiratie in intuneric."),
        MakeEvent(3, EventType::Scripted, 2, 35.0f, 2, "O usa se trinteste singura in spatele tau!"),
        MakeEvent(1, EventType::FalseAlarm, 1, 0.0f, 0, "Un zgomot dupa colt... dar nu e nimic."),
        MakeEvent(2, EventType::FalseAlarm, 2, 0.0f, 0, "O umbra se misca... a fost doar vantul."),
    };
    std::sort(events.begin(), events.end());

    story.lines.clear();
    story.AddLine(NarrationLine("Sunt Dima. Noaptea asta ma duc la Spitalul Psihiatric Gonjiam, ca sa arat ca nu e nimic infricosator acolo.", 4.0f, 0, Vec3(), 0));
    story.AddLine(NarrationLine("Se spune ca spitalul a fost construit in mijlocul padurii, departe de orice oras.", 4.0f, 0, Vec3(), 0));
    story.AddLine(NarrationLine("Oamenii zic ca de aceea locul e bantuit. Eu zic ca sunt doar povesti.", 4.0f, 1, Vec3(), 1));
    story.AddLine(NarrationLine("Holul asta pare mai lung decat ar trebui sa fie.", 3.0f, 2, Vec3(), 1));
    story.AddLine(NarrationLine("Subsolul. Aici s-ar fi intamplat cele mai ciudate lucruri, zice legenda.", 4.0f, 3, Vec3(), 2));

    currentLocationId = locations.begin()->first;
    safeCorners = 0;
    timeSinceLastScare = 0.0f;
    totalTime = 0.0f;
    state = GameState::Menu;
}

void GameEngine::Run()
{
    std::cout << "Game engine is running..." << std::endl;

    if (locations.empty())
    {
        std::cout << "Nu exista locatii: apeleaza Init() inainte de Run()." << std::endl;
        return;
    }

    running = true;
    state = GameState::Playing;
    deltaTime = 2.0f; // pentru demonstratie: intre doua locatii trec 2 secunde

    // Parcurgem locatiile in ordinea id-urilor (std::map e ordonat dupa cheie).
    for (const std::pair<const int, Location> &entry : locations)
    {
        const int id = entry.first;

        totalTime += deltaTime;
        timeSinceLastScare += deltaTime;
        player.Update(deltaTime);
        story.Update(deltaTime);

        ChangeLocation(id);

        renderer->Clear();
        renderer->DrawScene(FindLocation(id), player);

        // Cat timp mai sunt replici nejucate in locatia curenta, le afisam.
        for (std::optional<std::string> line = story.PlayNext(id); line; line = story.PlayNext(id))
            std::cout << "  [Dima] " << *line << '\n';

        const std::vector<Event> here = EventsInLocation(id);
        std::cout << "  Evenimente posibile aici: " << here.size() << '\n';

        for (const std::string &message : TriggerEvents())
            std::cout << "  ! " << message << '\n';

        renderer->DrawHUD(player);
        renderer->Present();
    }

    std::cout << "\nColturi linistite trecute: " << safeCorners << ", replici ramase: "
              << story.RemainingLines() << std::endl;

    // Salvam jocul intr-un fisier, il incarcam intr-un motor nou si comparam datele.
    const std::string savePath = "savegame.sav";
    if (SaveGame(savePath))
    {
        GameEngine loaded;
        const bool same = loaded.LoadGame(savePath) && loaded.player == player &&
                          loaded.locations == locations && loaded.events == events;
        std::cout << "Salvare in " << savePath << ": "
                  << (same ? "OK, datele se incarca identic." : "EROARE la verificare.") << std::endl;
    }
    else
    {
        std::cout << "Nu s-a putut scrie fisierul " << savePath << std::endl;
    }

    running = false;
}

void GameEngine::Shutdown() { running = false; }

// ---------- Cautari ----------

Location &GameEngine::FindLocation(int id)
{
    return locations.at(id); // std::out_of_range daca nu exista
}

Event &GameEngine::FindEvent(int id)
{
    std::vector<Event>::iterator it =
        std::find_if(events.begin(), events.end(), [id](const Event &e) { return e.id == id; });
    if (it == events.end())
        throw std::out_of_range("GameEngine: nu exista evenimentul " + std::to_string(id));
    return *it;
}

// std::copy_if copiaza in rezultat doar elementele care respecta conditia.
std::vector<Event> GameEngine::EventsInLocation(int locationId) const
{
    std::vector<Event> result;
    std::copy_if(events.begin(), events.end(), std::back_inserter(result),
                 [locationId](const Event &e) { return e.locationId == locationId; });
    return result;
}

// ---------- Logica de joc ----------

void GameEngine::ChangeLocation(int id)
{
    FindLocation(id).MarkVisited();
    currentLocationId = id;
}

std::vector<std::string> GameEngine::TriggerEvents()
{
    std::vector<std::string> messages;

    std::for_each(events.begin(), events.end(),
                  [this, &messages](Event &e)
                  {
                      if (e.locationId != currentLocationId || !e.ShouldTrigger(player.position, safeCorners))
                          return;

                      e.Trigger();
                      player.AddFear(e.fearAmount);

                      // Mecanica din GDD: alarmele false cresc contorul, scarul real il reseteaza.
                      if (e.type == EventType::FalseAlarm)
                      {
                          ++safeCorners;
                      }
                      else if (e.type == EventType::Scripted)
                      {
                          safeCorners = 0;
                          timeSinceLastScare = 0.0f;
                      }

                      if (!e.message.empty())
                          messages.push_back(e.message);
                  });

    return messages;
}

// ---------- Salvare / incarcare ----------

bool GameEngine::SaveGame(const std::string &path) const
{
    std::ofstream file(path);
    if (!file)
        return false;

    // max_digits10 garanteaza ca un float scris ca text se citeste inapoi identic.
    file << std::setprecision(std::numeric_limits<float>::max_digits10);

    file << player << '\n';
    file << currentLocationId << ' ' << safeCorners << ' ' << timeSinceLastScare << ' ' << totalTime << '\n';

    file << locations.size() << '\n';
    std::for_each(locations.begin(), locations.end(),
                  [&file](const std::pair<const int, Location> &entry) { file << entry.second << '\n'; });

    file << events.size() << '\n';
    std::for_each(events.begin(), events.end(), [&file](const Event &e) { file << e << '\n'; });

    file.flush();
    return file.good();
}

bool GameEngine::LoadGame(const std::string &path)
{
    std::ifstream file(path);
    if (!file)
        return false;

    // Citim totul in variabile locale; modificam motorul abia dupa ce citirea a reusit.
    Player loadedPlayer;
    int loadedLocationId = 0;
    int loadedCorners = 0;
    float loadedSinceScare = 0.0f;
    float loadedTotalTime = 0.0f;
    std::size_t locationCount = 0;
    std::size_t eventCount = 0;
    std::map<int, Location> loadedLocations;
    std::vector<Event> loadedEvents;

    file >> loadedPlayer >> loadedLocationId >> loadedCorners >> loadedSinceScare >> loadedTotalTime >>
        locationCount;

    for (std::size_t i = 0; i < locationCount && file; ++i)
    {
        Location location;
        if (file >> location)
            loadedLocations.emplace(location.id, location);
    }

    file >> eventCount;
    for (std::size_t i = 0; i < eventCount && file; ++i)
    {
        Event event;
        if (file >> event)
            loadedEvents.push_back(event);
    }

    if (!file || loadedLocations.size() != locationCount || loadedEvents.size() != eventCount)
        return false;

    player = loadedPlayer;
    locations = std::move(loadedLocations);
    events = std::move(loadedEvents);
    std::sort(events.begin(), events.end());
    currentLocationId = loadedLocationId;
    safeCorners = loadedCorners;
    timeSinceLastScare = loadedSinceScare;
    totalTime = loadedTotalTime;
    return true;
}