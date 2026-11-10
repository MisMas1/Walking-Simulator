#include "GameEngine.hpp"
#include "RaylibRenderer.hpp"
#include "Tools.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <cmath>

#include <sqlite3.h>

namespace
{
using DatabaseHandle = std::unique_ptr<sqlite3, decltype(&sqlite3_close)>;
using StatementHandle = std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>;

bool OpenDatabase(const std::string &path, int flags, DatabaseHandle &database)
{
    sqlite3 *handle = nullptr;
    const int result = sqlite3_open_v2(path.c_str(), &handle, flags, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "SQLite: nu se poate deschide " << path << ": "
                  << (handle ? sqlite3_errmsg(handle) : sqlite3_errstr(result)) << '\n';
        if (handle)
            sqlite3_close(handle);
        return false;
    }

    database.reset(handle);
    return true;
}

void ReportDatabaseError(const char *operation, sqlite3 *database)
{
    std::cerr << "SQLite: " << operation << ": " << sqlite3_errmsg(database) << '\n';
}
}

GameEngine::GameEngine() : renderer(std::make_unique<RaylibRenderer>()) {}

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
    player.position.y = 1.7f;
    player.position.z = 8.0f;
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
    locations.at(0).interactables.emplace_back(
        "Jurnal rupt", Vec3(3.0f, 0.8f, -6.0f), "In jurnal scrie: «Nu veni dupa mine la subsol.»");
    locations.at(1).interactables.emplace_back(
        "SpiritPod", Vec3(-4.0f, 0.9f, 1.0f), "Ai gasit un SpiritPod. Apasa E pentru a-l lua.");
    locations.at(2).interactables.emplace_back(
        "Bete de radiestezie", Vec3(4.0f, 0.9f, -2.0f), "Ai gasit bete de radiestezie. Apasa E pentru a le lua.");
    locations.at(3).interactables.emplace_back(
        "Dosar medical", Vec3(0.0f, 0.9f, -7.0f), "Dosarul este gol, cu exceptia unei fotografii arse.");

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
    if (locations.empty())
    {
        std::cout << "Nu exista locatii: apeleaza Init() inainte de Run()." << std::endl;
        return;
    }

    running = true;
    state = GameState::Playing;
    if (std::optional<std::string> line = story.PlayNext(currentLocationId))
    {
        renderer->currentSubtitle = *line;
        renderer->subtitleTimer = 6.0f;
    }

    while (running && !WindowShouldClose())
    {
        listener.Poll();
        if (listener.quit)
            break;

        deltaTime = std::min(GetFrameTime(), 0.05f);
        totalTime += deltaTime;
        timeSinceLastScare += deltaTime;

        player.Rotate(listener.mouseDeltaX * listener.mouseSensitivity,
                      -listener.mouseDeltaY * listener.mouseSensitivity);

        if (listener.flashlightToggle && player.HasTool(ToolType::Flashlight))
        {
            Tool &flashlight = player.GetTool(ToolType::Flashlight);
            flashlight.isActive = !flashlight.isActive;
            renderer->currentSubtitle = flashlight.isActive ? "Lanterna aprinsa." : "Lanterna stinsa.";
            renderer->subtitleTimer = 2.0f;
        }

        if (listener.prevTool)
            player.PrevTool();
        if (listener.nextTool)
            player.NextTool();
        if (listener.toolSelection >= 0 &&
            static_cast<std::size_t>(listener.toolSelection) < player.items.size())
            player.activeToolIndex = listener.toolSelection;

        if (listener.interact)
            InteractWithObject();
        if (listener.useTool)
            UseActiveTool();

        const float yaw = player.yaw * 0.017453292519943295f;
        Vec3 movement;
        if (listener.moveForward)
        {
            movement.x += std::sin(yaw);
            movement.z -= std::cos(yaw);
        }
        if (listener.moveBack)
        {
            movement.x -= std::sin(yaw);
            movement.z += std::cos(yaw);
        }
        if (listener.moveRight)
        {
            movement.x += std::cos(yaw);
            movement.z += std::sin(yaw);
        }
        if (listener.moveLeft)
        {
            movement.x -= std::cos(yaw);
            movement.z -= std::sin(yaw);
        }

        const float movementLength = std::sqrt(movement.x * movement.x + movement.z * movement.z);
        if (movementLength > 0.0f)
        {
            movement.x /= movementLength;
            movement.z /= movementLength;
            player.isSprinting = listener.sprint && player.stamina > 0.0f;
            player.Move(movement, deltaTime);
        }
        else
        {
            player.isSprinting = false;
        }

        player.position.x = std::clamp(player.position.x, -10.5f, 10.5f);
        if (player.position.z < -10.5f &&
            locations.at(currentLocationId).exits.find(currentLocationId + 1) !=
                locations.at(currentLocationId).exits.end())
        {
            ChangeLocation(currentLocationId + 1);
            player.position.z = 9.5f;
        }
        else if (player.position.z > 10.5f &&
                 locations.at(currentLocationId).exits.find(currentLocationId - 1) !=
                     locations.at(currentLocationId).exits.end())
        {
            ChangeLocation(currentLocationId - 1);
            player.position.z = -9.5f;
        }
        else
        {
            player.position.z = std::clamp(player.position.z, -10.5f, 10.5f);
        }

        player.Update(deltaTime);
        for (const std::unique_ptr<Tool> &tool : player.items)
        {
            if (tool)
            {
                tool->cooldownTimer = std::max(0.0f, tool->cooldownTimer - deltaTime);
                tool->Update(deltaTime);
            }
        }
        story.Update(deltaTime);
        renderer->subtitleTimer = std::max(0.0f, renderer->subtitleTimer - deltaTime);
        if (renderer->subtitleTimer <= 0.0f)
            renderer->currentSubtitle.clear();

        for (const std::string &message : TriggerEvents())
        {
            renderer->currentSubtitle = message;
            renderer->subtitleTimer = 4.0f;
        }

        renderer->currentLocationName = FindLocation(currentLocationId).name;
        renderer->Clear();
        renderer->DrawScene(FindLocation(currentLocationId), player);
        renderer->DrawHUD(player);
        renderer->Present();
    }

    running = false;
    Shutdown();

    const std::string savePath = "savegame.db";
    if (SaveGame(savePath))
        std::cout << "Joc salvat in " << savePath << ".\n";
    else
        std::cerr << "Nu s-a putut salva jocul in " << savePath << ".\n";
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
    if (std::optional<std::string> line = story.PlayNext(id))
    {
        renderer->currentSubtitle = *line;
        renderer->subtitleTimer = 6.0f;
    }
}

bool GameEngine::InteractWithObject()
{
    Location &location = FindLocation(currentLocationId);
    const float yaw = player.yaw * 0.017453292519943295f;
    const float pitch = player.pitch * 0.017453292519943295f;
    const float horizontalLook = std::cos(pitch);
    const Vec3 direction(std::sin(yaw) * horizontalLook, std::sin(pitch),
                         -std::cos(yaw) * horizontalLook);

    Interactable *target = nullptr;
    float nearestDistance = 3.5f;
    for (Interactable &item : location.interactables)
    {
        const Vec3 offset(item.position.x - player.position.x,
                          item.position.y - player.position.y,
                          item.position.z - player.position.z);
        const float projection = offset.x * direction.x + offset.y * direction.y +
                                 offset.z * direction.z;
        if (projection < 0.0f || projection > nearestDistance)
            continue;

        const float offsetLength = std::sqrt(offset.x * offset.x + offset.y * offset.y +
                                             offset.z * offset.z);
        const float perpendicularSquared = std::max(0.0f, offsetLength * offsetLength -
                                                             projection * projection);
        if (perpendicularSquared <= 0.8f * 0.8f)
        {
            nearestDistance = projection;
            target = &item;
        }
    }

    if (!target)
    {
        renderer->currentSubtitle = "Nu este nimic de atins in fata ta.";
        renderer->subtitleTimer = 2.0f;
        return false;
    }
    if (target->used)
    {
        renderer->currentSubtitle = "Ai verificat deja acest obiect.";
        renderer->subtitleTimer = 2.0f;
        return false;
    }

    target->used = true;
    story.AddLore(target->loreText);
    renderer->currentSubtitle = target->loreText;
    renderer->subtitleTimer = 5.0f;

    if (target->name == "SpiritPod")
    {
        player.AddTool(std::make_unique<SpiritPod>());
        renderer->currentSubtitle = "Ai luat SpiritPod. Selecteaza-l cu slotul 4.";
    }
    else if (target->name == "Bete de radiestezie")
    {
        player.AddTool(std::make_unique<DowsingRods>());
        renderer->currentSubtitle = "Ai luat betele de radiestezie. Selecteaza-le cu slotul 5.";
    }
    renderer->subtitleTimer = 5.0f;
    return true;
}

void GameEngine::UseActiveTool()
{
    if (player.items.empty() || player.activeToolIndex < 0 ||
        static_cast<std::size_t>(player.activeToolIndex) >= player.items.size() ||
        !player.items[player.activeToolIndex])
        return;

    Tool &tool = player.ActiveTool();
    Location &location = FindLocation(currentLocationId);
    if (tool.type == ToolType::Flashlight)
    {
        tool.isActive = !tool.isActive;
        renderer->currentSubtitle = tool.isActive ? "Lanterna aprinsa." : "Lanterna stinsa.";
    }
    else if (tool.type == ToolType::PolaroidCamera)
    {
        PolaroidCamera &camera = static_cast<PolaroidCamera &>(tool);
        if (camera.photosLeft <= 0)
        {
            renderer->currentSubtitle = "Aparatul nu mai are fotografii.";
        }
        else if (camera.cooldownTimer > 0.0f)
        {
            renderer->currentSubtitle = "Aparatul se reincarca.";
        }
        else
        {
            bool anomaly = false;
            for (const Event &event : events)
            {
                if (event.locationId == currentLocationId &&
                    Distance(player.position, event.triggerPosition) <= 8.0f)
                {
                    anomaly = true;
                    break;
                }
            }
            camera.photos.emplace_back(player.position, anomaly,
                                      anomaly ? "Anomalie detectata" : "Camera explorata");
            --camera.photosLeft;
            camera.cooldownTimer = camera.flashCooldown;
            renderer->currentSubtitle = anomaly ? "Fotografie facuta: anomalia a aparut pe film."
                                                : "Fotografie facuta. Nu se vede nimic neobisnuit.";
        }
    }
    else if (tool.type == ToolType::EMFMeter)
    {
        EMFMeter &meter = static_cast<EMFMeter &>(tool);
        float nearestEvent = meter.detectionRange;
        for (const Event &event : events)
        {
            if (event.locationId == currentLocationId)
                nearestEvent = std::min(nearestEvent, Distance(player.position, event.triggerPosition));
        }
        meter.currentLevel = nearestEvent < meter.detectionRange
                                 ? std::clamp(5 - static_cast<int>(nearestEvent / meter.detectionRange * 4.0f),
                                              1, 5)
                                 : 1;
        renderer->currentSubtitle = meter.currentLevel > 1
                                        ? "EMF: nivel " + std::to_string(meter.currentLevel) +
                                              " - activitate detectata."
                                        : "EMF: nivel 1 - semnal slab.";
    }
    else if (tool.type == ToolType::SpiritPod)
    {
        SpiritPod &pod = static_cast<SpiritPod &>(tool);
        pod.isListening = !pod.isListening;
        pod.isActive = pod.isListening;
        if (!pod.isListening)
        {
            renderer->currentSubtitle = "SpiritPod oprit.";
        }
        else
        {
            pod.lastResponse.clear();
            for (const Event &event : events)
            {
                if (event.locationId == currentLocationId &&
                    Distance(player.position, event.triggerPosition) <= pod.range)
                {
                    pod.lastResponse = event.message;
                    break;
                }
            }
            renderer->currentSubtitle = pod.lastResponse.empty()
                                            ? "SpiritPod asculta. Nu se aude niciun raspuns."
                                            : "SpiritPod: " + pod.lastResponse;
        }
    }
    else if (tool.type == ToolType::DowsingRods)
    {
        DowsingRods &rods = static_cast<DowsingRods &>(tool);
        const Interactable *nearest = nullptr;
        float nearestDistance = rods.detectionRange;
        for (const Interactable &item : location.interactables)
        {
            if (item.used)
                continue;
            const float distance = Distance(player.position, item.position);
            if (distance < nearestDistance)
            {
                nearest = &item;
                nearestDistance = distance;
            }
        }
        if (nearest)
        {
            rods.pointingAt = nearest->position;
            rods.angle = std::atan2(nearest->position.x - player.position.x,
                                    player.position.z - nearest->position.z) * 57.29577951308232f;
            renderer->currentSubtitle = "Betele indica o prezenta in apropiere.";
        }
        else
        {
            renderer->currentSubtitle = "Betele nu detecteaza nimic in apropiere.";
        }
    }
    renderer->subtitleTimer = 3.0f;
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
    std::ostringstream payload;

    // max_digits10 garanteaza ca un float scris ca text se citeste inapoi identic.
    payload << std::setprecision(std::numeric_limits<float>::max_digits10);

    payload << player << '\n';
    payload << currentLocationId << ' ' << safeCorners << ' ' << timeSinceLastScare << ' ' << totalTime << '\n';

    payload << locations.size() << '\n';
    std::for_each(locations.begin(), locations.end(),
                  [&payload](const std::pair<const int, Location> &entry) { payload << entry.second << '\n'; });

    payload << events.size() << '\n';
    std::for_each(events.begin(), events.end(), [&payload](const Event &e) { payload << e << '\n'; });

    if (!payload)
        return false;

    DatabaseHandle database(nullptr, &sqlite3_close);
    if (!OpenDatabase(path, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, database))
        return false;

    if (sqlite3_exec(database.get(), "BEGIN IMMEDIATE", nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        ReportDatabaseError("nu se poate incepe salvarea", database.get());
        return false;
    }

    const char *createTable =
        "CREATE TABLE IF NOT EXISTS game_state ("
        "id INTEGER PRIMARY KEY CHECK (id = 1), payload TEXT NOT NULL)";
    if (sqlite3_exec(database.get(), createTable, nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        ReportDatabaseError("nu se poate crea tabelul de salvare", database.get());
        sqlite3_exec(database.get(), "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
    }

    sqlite3_stmt *rawStatement = nullptr;
    const char *saveQuery =
        "INSERT OR REPLACE INTO game_state (id, payload) VALUES (1, ?1)";
    if (sqlite3_prepare_v2(database.get(), saveQuery, -1, &rawStatement, nullptr) != SQLITE_OK)
    {
        ReportDatabaseError("nu se poate pregati salvarea", database.get());
        sqlite3_exec(database.get(), "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
    }
    StatementHandle statement(rawStatement, &sqlite3_finalize);

    const std::string serialized = payload.str();
    if (sqlite3_bind_text(statement.get(), 1, serialized.c_str(),
                          static_cast<int>(serialized.size()), SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_step(statement.get()) != SQLITE_DONE)
    {
        ReportDatabaseError("nu se poate scrie starea jocului", database.get());
        statement.reset();
        sqlite3_exec(database.get(), "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
    }

    statement.reset();
    if (sqlite3_exec(database.get(), "COMMIT", nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        ReportDatabaseError("nu se poate confirma salvarea", database.get());
        sqlite3_exec(database.get(), "ROLLBACK", nullptr, nullptr, nullptr);
        return false;
    }

    return true;
}

bool GameEngine::LoadGame(const std::string &path)
{
    DatabaseHandle database(nullptr, &sqlite3_close);
    if (!OpenDatabase(path, SQLITE_OPEN_READWRITE, database))
        return false;

    sqlite3_stmt *rawStatement = nullptr;
    const char *loadQuery = "SELECT payload FROM game_state WHERE id = 1";
    if (sqlite3_prepare_v2(database.get(), loadQuery, -1, &rawStatement, nullptr) != SQLITE_OK)
    {
        ReportDatabaseError("nu se poate pregati incarcarea", database.get());
        return false;
    }
    StatementHandle statement(rawStatement, &sqlite3_finalize);

    if (sqlite3_step(statement.get()) != SQLITE_ROW)
        return false;

    const unsigned char *text = sqlite3_column_text(statement.get(), 0);
    const int textSize = sqlite3_column_bytes(statement.get(), 0);
    if (!text || textSize < 0)
        return false;

    std::istringstream file(std::string(reinterpret_cast<const char *>(text),
                                        static_cast<std::size_t>(textSize)));

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