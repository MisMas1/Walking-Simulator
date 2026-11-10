#include "RaylibRenderer.hpp"

#include <algorithm>
#include <cmath>
#include <string>

namespace
{
constexpr float RoomHalfSize = 12.0f;
constexpr float WallHeight = 3.6f;

RaylibColor RoomWallColor(int locationId)
{
    switch (locationId)
    {
    case 0:
        return RaylibColor{42, 48, 41, 255};
    case 1:
        return RaylibColor{55, 53, 43, 255};
    case 2:
        return RaylibColor{46, 48, 51, 255};
    default:
        return RaylibColor{41, 43, 49, 255};
    }
}

void DrawWall(float x, float z, float width, float depth, RaylibColor color)
{
    DrawCube(Vector3{x, WallHeight * 0.5f, z}, width, WallHeight, depth, color);
}

void DrawDoorwayWall(bool hasDoor, bool north, RaylibColor color)
{
    const float z = north ? -RoomHalfSize : RoomHalfSize;
    const float direction = north ? 1.0f : -1.0f;
    if (!hasDoor)
    {
        DrawWall(0.0f, z, RoomHalfSize * 2.0f, 0.6f, color);
        return;
    }

    DrawWall(-7.0f, z, 10.0f, 0.6f, color);
    DrawWall(7.0f, z, 10.0f, 0.6f, color);
    DrawCube(Vector3{0.0f, 2.7f, z}, 4.0f, 1.8f, 0.6f, color);
    DrawCube(Vector3{-2.15f, 1.4f, z}, 0.3f, 2.8f, 0.75f, RaylibColor{101, 82, 57, 255});
    DrawCube(Vector3{2.15f, 1.4f, z}, 0.3f, 2.8f, 0.75f, RaylibColor{101, 82, 57, 255});
    DrawCube(Vector3{0.0f, 2.9f, z}, 4.6f, 0.25f, 0.75f, RaylibColor{101, 82, 57, 255});
    DrawPlane(Vector3{0.0f, 0.01f, z + direction * 0.2f}, Vector2{3.6f, 0.45f},
              RaylibColor{62, 54, 42, 255});
}

void DrawRoomFurniture(int locationId)
{
    const RaylibColor wood{76, 62, 47, 255};
    if (locationId == 0)
    {
        DrawCube(Vector3{-7.2f, 1.0f, -2.0f}, 0.9f, 2.0f, 0.9f, RaylibColor{31, 51, 36, 255});
        DrawCube(Vector3{6.5f, 0.7f, 1.0f}, 2.0f, 1.4f, 1.4f, RaylibColor{38, 53, 37, 255});
        DrawCube(Vector3{3.0f, 0.45f, -6.0f}, 2.5f, 0.9f, 0.5f, wood);
    }
    else if (locationId == 1)
    {
        DrawCube(Vector3{-7.0f, 0.7f, 2.0f}, 2.4f, 1.4f, 1.1f, wood);
        DrawCube(Vector3{7.0f, 1.1f, -4.0f}, 1.2f, 2.2f, 1.2f, RaylibColor{66, 64, 53, 255});
    }
    else if (locationId == 2)
    {
        DrawCube(Vector3{-7.0f, 0.6f, -2.0f}, 2.6f, 1.2f, 1.2f, RaylibColor{58, 59, 58, 255});
        DrawCube(Vector3{7.0f, 0.8f, 2.0f}, 2.0f, 1.6f, 1.2f, wood);
        DrawCube(Vector3{-4.0f, 1.8f, -7.0f}, 0.5f, 3.6f, 0.3f, RaylibColor{69, 72, 72, 255});
    }
    else
    {
        DrawCube(Vector3{-7.0f, 0.8f, 1.0f}, 2.0f, 1.6f, 1.2f, RaylibColor{65, 66, 70, 255});
        DrawCube(Vector3{7.0f, 1.0f, -3.0f}, 1.0f, 2.0f, 1.0f, RaylibColor{49, 53, 62, 255});
        DrawCube(Vector3{0.0f, 0.4f, -7.0f}, 3.2f, 0.8f, 1.0f, wood);
    }
}

void DrawInteractable(const Interactable &item)
{
    if (item.used)
        return;

    const Vector3 position{item.position.x, item.position.y, item.position.z};
    const RaylibColor objectColor = item.name.find("Spirit") != std::string::npos
                                  ? RaylibColor{77, 117, 142, 255}
                                  : item.name.find("Bete") != std::string::npos
                                        ? RaylibColor{151, 110, 63, 255}
                                        : RaylibColor{125, 108, 82, 255};
    DrawCube(position, 0.65f, 0.75f, 0.5f, objectColor);
    DrawCubeWires(position, 0.68f, 0.78f, 0.53f, RaylibColor{190, 166, 119, 255});
}
}

RaylibRenderer::~RaylibRenderer()
{
    if (initialized)
        CloseWindow();
}

bool RaylibRenderer::Init()
{
    if (windowTitle.empty())
        windowTitle = "Gonjiam: Ghost Hunt";

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, windowTitle.c_str());
    if (!IsWindowReady())
        return false;

    initialized = true;
    SetTargetFPS(60);
    DisableCursor();
    camera.position = Vector3{0.0f, 1.7f, 8.0f};
    camera.target = Vector3{0.0f, 1.7f, 7.0f};
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 75.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    return true;
}

void RaylibRenderer::Clear()
{
    BeginDrawing();
    ClearBackground(RaylibColor{9, 12, 14, 255});
}

void RaylibRenderer::Present() { EndDrawing(); }

void RaylibRenderer::DrawScene(const Location &location, const Player &player)
{
    const float yaw = player.yaw * DEG2RAD;
    const float pitch = player.pitch * DEG2RAD;
    const float horizontalLook = std::cos(pitch);

    camera.position = Vector3{player.position.x, player.position.y, player.position.z};
    camera.target = Vector3{
        player.position.x + std::sin(yaw) * horizontalLook,
        player.position.y + std::sin(pitch),
        player.position.z - std::cos(yaw) * horizontalLook};

    BeginMode3D(camera);
    const bool isForest = location.id == 0;
    DrawCube(Vector3{0.0f, -0.3f, 0.0f}, RoomHalfSize * 2.0f, 0.6f,
             RoomHalfSize * 2.0f, isForest ? RaylibColor{48, 47, 39, 255}
                                            : RaylibColor{49, 48, 44, 255});
    if (isForest)
    {
        DrawCube(Vector3{0.0f, 0.04f, 0.0f}, 3.8f, 0.08f, RoomHalfSize * 2.0f,
                 RaylibColor{79, 69, 54, 255});

        const Vector2 treePositions[] = {
            {-9.0f, -8.0f}, {-6.0f, -2.0f}, {-8.0f, 6.0f}, {-4.0f, 9.0f},
            {8.0f, -9.0f}, {6.5f, -4.0f}, {9.0f, 3.0f}, {5.0f, 8.0f}};
        for (const Vector2 &tree : treePositions)
        {
            DrawCylinder(Vector3{tree.x, 1.8f, tree.y}, 0.2f, 0.38f, 3.6f, 8,
                         RaylibColor{53, 43, 34, 255});
            DrawSphere(Vector3{tree.x, 4.0f, tree.y}, 1.8f,
                       RaylibColor{29, 48, 34, 255});
        }
    }
    else
    {
        DrawCubeWires(Vector3{0.0f, 3.7f, 0.0f}, RoomHalfSize * 2.0f, 0.3f,
                      RoomHalfSize * 2.0f, RaylibColor{40, 42, 43, 255});

        const RaylibColor wallColor = RoomWallColor(location.id);
        DrawWall(-RoomHalfSize, 0.0f, 0.6f, RoomHalfSize * 2.0f, wallColor);
        DrawWall(RoomHalfSize, 0.0f, 0.6f, RoomHalfSize * 2.0f, wallColor);
        DrawDoorwayWall(location.exits.find(location.id + 1) != location.exits.end(), true, wallColor);
        DrawDoorwayWall(location.exits.find(location.id - 1) != location.exits.end(), false, wallColor);
    }

    DrawRoomFurniture(location.id);
    for (const Interactable &item : location.interactables)
        DrawInteractable(item);

    if (player.HasTool(ToolType::Flashlight) &&
        player.GetTool(ToolType::Flashlight).isActive)
    {
        const Vector3 beamStart = camera.position;
        const Vector3 beamEnd = Vector3{
            camera.position.x + std::sin(yaw) * 7.0f,
            camera.position.y + std::sin(pitch) * 7.0f,
            camera.position.z - std::cos(yaw) * 7.0f};
        DrawCylinderEx(beamStart, beamEnd, 0.42f, 1.6f, 12, RaylibColor{180, 174, 135, 32});
    }

    if (player.fear > 60.0f)
        DrawSphere(Vector3{0.0f, 2.0f, -7.0f}, 0.8f, RaylibColor{52, 48, 53, 255});

    EndMode3D();
}

void RaylibRenderer::DrawHUD(const Player &player)
{
    if (!showHUD)
        return;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawRectangle(18, 18, 310, 100, RaylibColor{8, 10, 12, 205});
    DrawText(currentLocationName.empty() ? "GONJIAM  |  EXPLORARE" : currentLocationName.c_str(),
             32, 30, 20, RaylibColor{219, 211, 190, 255});
    DrawText(TextFormat("Frica: %d%%", static_cast<int>(player.fear)), 32, 62, 18,
             RaylibColor{205, 126, 112, 255});
    DrawText(TextFormat("Stamina: %d%%", static_cast<int>(player.stamina)), 166, 62, 18,
             RaylibColor{160, 192, 154, 255});

    DrawRectangle(screenWidth / 2 - 7, screenHeight / 2 - 1, 14, 2, RaylibColor{225, 219, 194, 210});
    DrawRectangle(screenWidth / 2 - 1, screenHeight / 2 - 7, 2, 14, RaylibColor{225, 219, 194, 210});

    const std::string toolName = player.items.empty() ? "Niciun obiect" : player.ActiveTool().name;
    DrawRectangle(18, screenHeight - 80, 440, 60, RaylibColor{8, 10, 12, 205});
    DrawText(TextFormat("Unealta: %s  |  Slot %d/%d", toolName.c_str(),
                        player.activeToolIndex + 1, static_cast<int>(player.items.size())),
             32, screenHeight - 68, 18, RaylibColor{219, 211, 190, 255});
    DrawText("WASD miscare  SHIFT sprint  MOUSE privire  E interactiune  CLICK foloseste",
             32, screenHeight - 42, 14, RaylibColor{172, 171, 162, 255});
    DrawText("F lanterna  Q/R sau rotita mouse-ului schimba unealta  ESC iesire",
             32, screenHeight - 24, 14, RaylibColor{172, 171, 162, 255});

    if (player.HasTool(ToolType::Flashlight) &&
        player.GetTool(ToolType::Flashlight).isActive)
        DrawText("LANTERNA APRINSA", screenWidth - 200, 30, 18, RaylibColor{223, 201, 125, 255});
    else
        DrawText("LANTERNA STINSA", screenWidth - 200, 30, 18, RaylibColor{143, 143, 137, 255});

    if (!currentSubtitle.empty())
    {
        const int textWidth = MeasureText(currentSubtitle.c_str(), 20);
        DrawRectangle(screenWidth / 2 - textWidth / 2 - 18, screenHeight - 145,
                      textWidth + 36, 40, RaylibColor{8, 10, 12, 220});
        DrawText(currentSubtitle.c_str(), screenWidth / 2 - textWidth / 2,
                 screenHeight - 134, 20, RaylibColor{226, 218, 198, 255});
    }
}
