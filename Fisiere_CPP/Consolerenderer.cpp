#include "ConsoleRenderer.hpp"

#include <algorithm>
#include <iterator>
#include <string>

ConsoleRenderer::ConsoleRenderer(std::ostream &_out) : out(_out) {}

bool ConsoleRenderer::Init()
{
    if (windowTitle.empty())
        windowTitle = "Gonjiam: Ghost Hunt";

    out << "[Renderer] " << windowTitle << " (" << windowWidth << 'x' << windowHeight << ")\n";
    return out.good();
}

void ConsoleRenderer::Clear() { out << '\n'; }

void ConsoleRenderer::Present() { out << std::flush; }

void ConsoleRenderer::DrawScene(const Location &location, const Player &player)
{
    out << "=== " << location.name << " ===\n";
    out << location.description << '\n';
    out << (location.isDark ? "Locatie intunecata: ai nevoie de lanterna." : "Locatie luminata.") << '\n';

    out << "Iesiri spre locatiile: ";
    std::copy(location.exits.begin(), location.exits.end(), std::ostream_iterator<int>(out, " "));
    out << '\n';

    out << "Pozitie jucator: " << player.position << '\n';
}

void ConsoleRenderer::DrawHUD(const Player &player)
{
    if (!showHUD)
        return;

    out << "[HUD] Frica " << player.fear << '/' << player.maxFear << " | Stamina " << player.stamina << '/'
        << player.maxStamina << " | Unealta: ";

    const bool hasActiveTool = player.activeToolIndex >= 0 &&
                               static_cast<std::size_t>(player.activeToolIndex) < player.items.size() &&
                               player.items[player.activeToolIndex];
    if (hasActiveTool)
        out << player.ActiveTool().name;
    else
        out << "-";

    out << '\n';
}