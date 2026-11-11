#include "RaylibRenderer.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <string>

namespace
{
constexpr float RoomHalfSize = 12.0f;
constexpr float ForestHalfSize = 30.0f;
constexpr float WallHeight = 3.8f;

const char *LightingFragmentShader = R"(
#version 330
in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;
uniform sampler2D texture0;
uniform float flashlightOn;
uniform float screenAspect;
void main()
{
    vec4 scene = texture(texture0, fragTexCoord);
    vec2 centered = (fragTexCoord - vec2(0.5)) * vec2(screenAspect, 1.0);
    float distanceFromCenter = length(centered);
    float light = 1.0 - smoothstep(0.21, 0.555, distanceFromCenter);
    float illumination = mix(0.035, 1.0, light * flashlightOn);
    finalColor = vec4(scene.rgb * illumination, scene.a);
}
)";

RaylibColor RoomWallColor(int locationId)
{
    switch (locationId)
    {
    case 2:
        return RaylibColor{61, 58, 50, 255};
    case 3:
        return RaylibColor{54, 57, 57, 255};
    case 4:
        return RaylibColor{57, 50, 55, 255};
    case 5:
        return RaylibColor{62, 56, 47, 255};
    case 6:
        return RaylibColor{55, 57, 60, 255};
    case 7:
        return RaylibColor{51, 56, 59, 255};
    case 8:
        return RaylibColor{56, 48, 57, 255};
    default:
        return RaylibColor{49, 51, 53, 255};
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
    DrawCube(Vector3{0.0f, 2.8f, z}, 4.0f, 1.8f, 0.6f, color);
    DrawCube(Vector3{-2.15f, 1.4f, z}, 0.3f, 2.8f, 0.75f, RaylibColor{85, 69, 51, 255});
    DrawCube(Vector3{2.15f, 1.4f, z}, 0.3f, 2.8f, 0.75f, RaylibColor{85, 69, 51, 255});
    DrawCube(Vector3{0.0f, 2.9f, z}, 4.6f, 0.25f, 0.75f, RaylibColor{85, 69, 51, 255});
    DrawPlane(Vector3{0.0f, 0.01f, z + direction * 0.2f}, Vector2{3.6f, 0.45f},
              RaylibColor{62, 54, 42, 255});
}

void DrawForest()
{
    DrawCube(Vector3{0.0f, -0.3f, 0.0f}, ForestHalfSize * 2.0f, 0.6f,
             ForestHalfSize * 2.0f, RaylibColor{91, 67, 43, 255});
    DrawCube(Vector3{0.0f, 0.035f, 0.0f}, 5.0f, 0.07f, ForestHalfSize * 2.0f,
             RaylibColor{126, 91, 53, 255});

    const Vector3 soilPatches[] = {
        {-12.0f, 0.025f, -22.0f}, {9.0f, 0.025f, -18.0f}, {-8.0f, 0.025f, -8.0f},
        {13.0f, 0.025f, -3.0f}, {-15.0f, 0.025f, 8.0f}, {10.0f, 0.025f, 13.0f},
        {-11.0f, 0.025f, 22.0f}, {17.0f, 0.025f, 24.0f}};
    for (std::size_t i = 0; i < std::size(soilPatches); ++i)
    {
        const float width = 2.5f + static_cast<float>(i % 3);
        const float depth = 1.5f + static_cast<float>((i + 1) % 3);
        const RaylibColor color = (i % 2 == 0) ? RaylibColor{105, 78, 49, 255}
                                               : RaylibColor{78, 61, 43, 255};
        DrawCube(soilPatches[i], width, 0.05f, depth, color);
    }

    const Vector2 treePositions[] = {
        {-24.0f, -24.0f}, {-19.0f, -12.0f}, {-26.0f, 3.0f}, {-17.0f, 18.0f},
        {-23.0f, 25.0f}, {24.0f, -24.0f}, {18.0f, -14.0f}, {27.0f, 1.0f},
        {19.0f, 17.0f}, {25.0f, 25.0f}, {-10.0f, -20.0f}, {11.0f, 22.0f}};
    for (const Vector2 &tree : treePositions)
    {
        DrawCylinderEx(Vector3{tree.x, 0.02f, tree.y},
                       Vector3{tree.x, 5.0f, tree.y}, 0.48f, 0.29f, 10,
                       RaylibColor{72, 49, 33, 255});
        DrawSphere(Vector3{tree.x, 5.1f, tree.y}, 2.35f,
                   RaylibColor{35, 61, 39, 255});
        DrawSphere(Vector3{tree.x - 1.25f, 4.65f, tree.y + 0.25f}, 1.55f,
                   RaylibColor{31, 55, 36, 255});
        DrawSphere(Vector3{tree.x + 1.2f, 4.75f, tree.y - 0.2f}, 1.65f,
                   RaylibColor{38, 65, 41, 255});
    }

    DrawCube(Vector3{-3.2f, 1.8f, -29.5f}, 0.45f, 3.6f, 0.5f,
             RaylibColor{78, 64, 46, 255});
    DrawCube(Vector3{3.2f, 1.8f, -29.5f}, 0.45f, 3.6f, 0.5f,
             RaylibColor{78, 64, 46, 255});
    DrawCube(Vector3{0.0f, 3.6f, -29.5f}, 6.8f, 0.4f, 0.5f,
             RaylibColor{78, 64, 46, 255});
    DrawCube(Vector3{0.0f, 1.6f, -29.55f}, 0.12f, 3.2f, 0.2f,
             RaylibColor{110, 93, 66, 255});
}

void DrawHatchFloor(RaylibColor floor)
{
    DrawCube(Vector3{0.0f, -0.3f, -6.75f}, 24.0f, 0.6f, 10.5f, floor);
    DrawCube(Vector3{0.0f, -0.3f, 6.75f}, 24.0f, 0.6f, 10.5f, floor);
    DrawCube(Vector3{-6.75f, -0.3f, 0.0f}, 10.5f, 0.6f, 3.0f, floor);
    DrawCube(Vector3{6.75f, -0.3f, 0.0f}, 10.5f, 0.6f, 3.0f, floor);
    DrawCubeWires(Vector3{0.0f, -0.05f, 0.0f}, 3.0f, 0.15f, 3.0f,
                  RaylibColor{113, 92, 63, 255});
    DrawCube(Vector3{-1.65f, 0.45f, 0.0f}, 0.3f, 1.0f, 3.3f,
             RaylibColor{88, 70, 50, 255});
    DrawCube(Vector3{1.65f, 0.45f, 0.0f}, 0.3f, 1.0f, 3.3f,
             RaylibColor{88, 70, 50, 255});
    for (int step = 0; step < 5; ++step)
    {
        const float z = 1.2f + step * 0.42f;
        const float y = -0.1f - step * 0.32f;
        DrawCube(Vector3{0.0f, y, z}, 2.7f, 0.12f, 0.35f,
                 RaylibColor{90, 72, 53, 255});
    }
}

void DrawBasementStairs()
{
    for (int step = 0; step < 8; ++step)
    {
        DrawCube(Vector3{0.0f, 0.2f + step * 0.32f, 8.5f - step * 0.72f},
                 3.4f, 0.18f, 0.8f, RaylibColor{84, 70, 53, 255});
    }
    DrawCube(Vector3{-1.8f, 1.3f, 6.0f}, 0.18f, 2.6f, 5.5f,
             RaylibColor{75, 66, 55, 255});
    DrawCube(Vector3{1.8f, 1.3f, 6.0f}, 0.18f, 2.6f, 5.5f,
             RaylibColor{75, 66, 55, 255});
}

void DrawRoomFurniture(int locationId)
{
    const RaylibColor wood{76, 62, 47, 255};
    switch (locationId)
    {
    case 1:
        DrawCube(Vector3{-7.0f, 0.7f, 2.0f}, 2.4f, 1.4f, 1.1f, wood);
        DrawCube(Vector3{7.0f, 1.1f, -4.0f}, 1.2f, 2.2f, 1.2f,
                 RaylibColor{66, 64, 53, 255});
        break;
    case 2:
        DrawCube(Vector3{-7.0f, 0.6f, -2.0f}, 2.6f, 1.2f, 1.2f,
                 RaylibColor{58, 59, 58, 255});
        DrawCube(Vector3{7.0f, 0.8f, 2.0f}, 2.0f, 1.6f, 1.2f, wood);
        break;
    case 3:
        DrawCube(Vector3{-7.0f, 1.4f, -2.0f}, 1.0f, 2.8f, 1.0f,
                 RaylibColor{65, 66, 63, 255});
        DrawCube(Vector3{7.0f, 1.4f, 4.0f}, 1.0f, 2.8f, 1.0f,
                 RaylibColor{65, 66, 63, 255});
        break;
    case 4:
        DrawCube(Vector3{-6.0f, 0.7f, -1.0f}, 3.5f, 1.4f, 2.0f,
                 RaylibColor{64, 59, 62, 255});
        DrawCube(Vector3{6.0f, 0.7f, 4.0f}, 3.5f, 1.4f, 2.0f,
                 RaylibColor{64, 59, 62, 255});
        DrawCube(Vector3{0.0f, 1.4f, -8.0f}, 1.0f, 2.8f, 0.35f,
                 RaylibColor{73, 72, 66, 255});
        break;
    case 5:
        DrawCube(Vector3{0.0f, 0.75f, 0.0f}, 5.0f, 0.22f, 2.3f, wood);
        for (float x : {-2.0f, 2.0f})
            for (float z : {-0.8f, 0.8f})
                DrawCube(Vector3{x, 0.35f, z}, 0.2f, 0.8f, 0.2f, wood);
        break;
    case 6:
        DrawHatchFloor(RoomWallColor(locationId));
        DrawCube(Vector3{-8.0f, 1.0f, -3.0f}, 2.0f, 2.0f, 1.2f,
                 RaylibColor{61, 63, 64, 255});
        break;
    case 7:
        for (float x : {-8.0f, -4.5f, 4.5f, 8.0f})
            DrawCube(Vector3{x, 1.5f, -2.0f}, 2.0f, 3.0f, 0.7f, wood);
        break;
    case 8:
        DrawCube(Vector3{0.0f, 0.8f, 0.0f}, 2.0f, 1.6f, 2.0f,
                 RaylibColor{61, 54, 63, 255});
        break;
    case 9:
        DrawBasementStairs();
        break;
    default:
        break;
    }
}

void DrawInteractable(const Interactable &item)
{
    if (item.used)
        return;
    if (item.name.find("Trapa") != std::string::npos)
        return;

    const Vector3 position{item.position.x, item.position.y, item.position.z};
    RaylibColor objectColor{125, 108, 82, 255};
    if (item.name.find("Spirit") != std::string::npos)
        objectColor = RaylibColor{77, 117, 142, 255};
    else if (item.name.find("Bete") != std::string::npos)
        objectColor = RaylibColor{151, 110, 63, 255};
    else if (item.name.find("Baterii") != std::string::npos)
        objectColor = RaylibColor{191, 170, 72, 255};
    else if (item.name.find("EMF") != std::string::npos)
        objectColor = RaylibColor{80, 134, 89, 255};
    else if (item.name.find("Polaroid") != std::string::npos)
        objectColor = RaylibColor{68, 79, 91, 255};

    DrawCube(position, 0.65f, 0.75f, 0.5f, objectColor);
    DrawCubeWires(position, 0.68f, 0.78f, 0.53f, RaylibColor{190, 166, 119, 255});
}

void DrawGhost(const Vec3 &position)
{
    const RaylibColor ghostColor{190, 212, 223, 112};
    DrawCylinderEx(Vector3{position.x, position.y + 0.06f, position.z},
                   Vector3{position.x, position.y + 1.45f, position.z},
                   0.38f, 0.12f, 12, ghostColor);
    DrawSphere(Vector3{position.x, position.y + 1.7f, position.z}, 0.34f, ghostColor);
}

void DrawPlacedRods(const Vec3 &position)
{
    const Vector3 center{position.x, position.y + 0.12f, position.z};
    DrawCylinderEx(Vector3{center.x - 0.18f, center.y - 0.1f, center.z},
                   Vector3{center.x - 0.45f, center.y + 0.35f, center.z - 0.55f},
                   0.035f, 0.025f, 8, RaylibColor{190, 158, 95, 255});
    DrawCylinderEx(Vector3{center.x + 0.18f, center.y - 0.1f, center.z},
                   Vector3{center.x + 0.45f, center.y + 0.35f, center.z - 0.55f},
                   0.035f, 0.025f, 8, RaylibColor{190, 158, 95, 255});
}

void DrawDetectionRadius(const Vec3 &position)
{
    const Vector3 center{position.x, 0.035f, position.z};
    constexpr float radius = 2.8f;
    DrawCircle3D(center, radius, Vector3{1.0f, 0.0f, 0.0f}, 90.0f,
                 RaylibColor{255, 202, 48, 34});

    constexpr int segments = 64;
    for (int i = 0; i < segments; ++i)
    {
        const float angle0 = static_cast<float>(i) * 2.0f * PI / segments;
        const float angle1 = static_cast<float>(i + 1) * 2.0f * PI / segments;
        DrawLine3D(Vector3{center.x + std::cos(angle0) * radius, center.y + 0.01f,
                           center.z + std::sin(angle0) * radius},
                   Vector3{center.x + std::cos(angle1) * radius, center.y + 0.01f,
                           center.z + std::sin(angle1) * radius},
                   RaylibColor{255, 211, 64, 230});
    }
}
}

RaylibRenderer::~RaylibRenderer()
{
    if (shaderLoaded)
        UnloadShader(lightingShader);
    if (sceneTarget.id != 0)
        UnloadRenderTexture(sceneTarget);
    if (initialized)
        CloseWindow();
}

Vector2 RaylibRenderer::CalculateScreenResolution()
{
    const int monitor = std::max(0, GetCurrentMonitor());
    int width = GetMonitorWidth(monitor);
    int height = GetMonitorHeight(monitor);
    if (width <= 0 || height <= 0)
    {
        width = 1280;
        height = 720;
    }
    return Vector2{static_cast<float>(width), static_cast<float>(height)};
}

bool RaylibRenderer::Init()
{
    if (windowTitle.empty())
        windowTitle = "Gonjiam: Ghost Hunt";

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_UNDECORATED);
    InitWindow(windowWidth, windowHeight, windowTitle.c_str());
    if (!IsWindowReady())
        return false;

    initialized = true;
    const Vector2 resolution = CalculateScreenResolution();
    windowWidth = static_cast<int>(resolution.x);
    windowHeight = static_cast<int>(resolution.y);
    SetWindowPosition(0, 0);
    SetWindowSize(windowWidth, windowHeight);
    fullscreen = true;

    sceneTarget = LoadRenderTexture(windowWidth, windowHeight);
    if (sceneTarget.id == 0)
    {
        TraceLog(LOG_WARNING, "Textura pentru iluminare nu este disponibila; folosesc iluminarea de rezerva.");
    }

    if (sceneTarget.id != 0)
    {
        lightingShader = LoadShaderFromMemory(nullptr, LightingFragmentShader);
        shaderLoaded = lightingShader.id != 0;
    }
    if (shaderLoaded)
    {
        flashlightUniform = GetShaderLocation(lightingShader, "flashlightOn");
        screenAspectUniform = GetShaderLocation(lightingShader, "screenAspect");
    }

    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    EnableCursor();
    SetMousePosition(257, static_cast<int>(GetScreenHeight() * 0.58f + 29.0f));
    camera.position = Vector3{0.0f, 1.7f, 8.0f};
    camera.target = Vector3{0.0f, 1.7f, 7.0f};
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 75.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    return true;
}

void RaylibRenderer::SetMouseCaptured(bool captured)
{
    if (captured)
    {
        DisableCursor();
    }
    else
    {
        EnableCursor();
        SetMousePosition(257, static_cast<int>(GetScreenHeight() * 0.58f + 29.0f));
    }
}

void RaylibRenderer::Clear()
{
    if (sceneTarget.id != 0)
    {
        BeginTextureMode(sceneTarget);
        ClearBackground(RaylibColor{9, 12, 14, 255});
    }
    else
    {
        BeginDrawing();
        ClearBackground(RaylibColor{9, 12, 14, 255});
    }
}

void RaylibRenderer::Present()
{
    if (sceneTarget.id != 0)
    {
        EndTextureMode();
        BeginDrawing();

        const Rectangle source{0.0f, 0.0f, static_cast<float>(sceneTarget.texture.width),
                               -static_cast<float>(sceneTarget.texture.height)};
        const Rectangle destination{0.0f, 0.0f, static_cast<float>(GetScreenWidth()),
                                    static_cast<float>(GetScreenHeight())};
        if (shaderLoaded)
        {
            const float flashlightValue = flashlightEnabled ? 1.0f : 0.0f;
            const float aspect = static_cast<float>(GetScreenWidth()) /
                                 static_cast<float>(std::max(1, GetScreenHeight()));
            SetShaderValue(lightingShader, flashlightUniform, &flashlightValue, SHADER_UNIFORM_FLOAT);
            SetShaderValue(lightingShader, screenAspectUniform, &aspect, SHADER_UNIFORM_FLOAT);
            BeginShaderMode(lightingShader);
        }
        DrawTexturePro(sceneTarget.texture, source, destination, Vector2{0.0f, 0.0f}, 0.0f,
                       RaylibColor{255, 255, 255, 255});
        if (shaderLoaded)
            EndShaderMode();
    }

    if (!shaderLoaded)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                      RaylibColor{0, 0, 0, 225});
        if (flashlightEnabled)
        {
            BeginBlendMode(BLEND_ADDITIVE);
            DrawCircleGradient(GetScreenWidth() / 2, GetScreenHeight() / 2,
                               static_cast<float>(GetScreenHeight()) * 0.38f,
                               RaylibColor{112, 104, 80, 235}, RaylibColor{0, 0, 0, 0});
            EndBlendMode();
        }
    }
    DrawDetectionOverlay();
    DrawGameHUD();
    EndDrawing();
}

void RaylibRenderer::DrawScene(const Location &location, const Player &player)
{
    detectionOverlayVisible = rodsPlaced && location.id == rodsLocationId;
    const float yaw = player.yaw * DEG2RAD;
    const float pitch = player.pitch * DEG2RAD;
    const float horizontalLook = std::cos(pitch);

    camera.position = Vector3{player.position.x, player.position.y, player.position.z};
    camera.target = Vector3{
        player.position.x + std::sin(yaw) * horizontalLook,
        player.position.y + std::sin(pitch),
        player.position.z - std::cos(yaw) * horizontalLook};

    BeginMode3D(camera);
    if (location.id == 0)
    {
        DrawForest();
    }
    else
    {
        const RaylibColor wallColor = location.id == 9
                                          ? RaylibColor{42, 43, 47, 255}
                                          : RoomWallColor(location.id);
        if (location.id != 6)
            DrawCube(Vector3{0.0f, -0.3f, 0.0f}, RoomHalfSize * 2.0f, 0.6f,
                     RoomHalfSize * 2.0f, wallColor);
        DrawCube(Vector3{0.0f, 3.9f, 0.0f}, RoomHalfSize * 2.0f, 0.3f,
                 RoomHalfSize * 2.0f, wallColor);
        DrawWall(-RoomHalfSize, 0.0f, 0.6f, RoomHalfSize * 2.0f, wallColor);
        DrawWall(RoomHalfSize, 0.0f, 0.6f, RoomHalfSize * 2.0f, wallColor);
        DrawDoorwayWall(location.exits.find(location.id + 1) != location.exits.end(),
                        true, wallColor);
        DrawDoorwayWall(location.exits.find(location.id - 1) != location.exits.end(),
                        false, wallColor);
        DrawRoomFurniture(location.id);
    }

    for (const Interactable &item : location.interactables)
        DrawInteractable(item);
    if (rodsPlaced && location.id == rodsLocationId)
    {
        DrawDetectionRadius(rodsPosition);
        DrawPlacedRods(rodsPosition);
    }
    if (ghostVisible)
        DrawGhost(ghostPosition);
    EndMode3D();

    flashlightEnabled = player.HasTool(ToolType::Flashlight) &&
                        player.GetTool(ToolType::Flashlight).isActive &&
                        player.GetTool(ToolType::Flashlight).battery > 0.0f &&
                        flashlightFlickerTimer <= 0.0f;
}

void RaylibRenderer::DrawDetectionOverlay()
{
    if (!detectionOverlayVisible)
        return;

    constexpr float radius = 2.8f;
    constexpr int segments = 64;
    const Vector3 forward{camera.target.x - camera.position.x,
                          camera.target.y - camera.position.y,
                          camera.target.z - camera.position.z};
    const Vector3 cameraPosition = camera.position;
    const auto inFront = [&forward, &cameraPosition](const Vector3 &point)
    {
        return (point.x - cameraPosition.x) * forward.x +
                   (point.y - cameraPosition.y) * forward.y +
                   (point.z - cameraPosition.z) * forward.z >
               0.05f;
    };

    for (int i = 0; i < segments; ++i)
    {
        const float angle0 = static_cast<float>(i) * 2.0f * PI / segments;
        const float angle1 = static_cast<float>(i + 1) * 2.0f * PI / segments;
        const Vector3 world0{rodsPosition.x + std::cos(angle0) * radius, 0.06f,
                             rodsPosition.z + std::sin(angle0) * radius};
        const Vector3 world1{rodsPosition.x + std::cos(angle1) * radius, 0.06f,
                             rodsPosition.z + std::sin(angle1) * radius};
        if (!inFront(world0) || !inFront(world1))
            continue;

        const Vector2 screen0 = GetWorldToScreen(world0, camera);
        const Vector2 screen1 = GetWorldToScreen(world1, camera);
        const float width = static_cast<float>(GetScreenWidth());
        const float height = static_cast<float>(GetScreenHeight());
        if (screen0.x < -width * 2.0f || screen0.x > width * 3.0f ||
            screen1.x < -width * 2.0f || screen1.x > width * 3.0f ||
            screen0.y < -height * 2.0f || screen0.y > height * 3.0f ||
            screen1.y < -height * 2.0f || screen1.y > height * 3.0f)
            continue;
        DrawLineEx(screen0, screen1, 2.5f, RaylibColor{255, 211, 64, 190});
    }
}

void RaylibRenderer::DrawHUD(const Player &player)
{
    if (!showHUD)
        return;

    fearAmount = player.fear;
    staminaAmount = player.stamina;
    if (!player.items.empty() && player.activeToolIndex >= 0 &&
        static_cast<std::size_t>(player.activeToolIndex) < player.items.size() &&
        player.items[player.activeToolIndex])
    {
        activeToolName = player.ActiveTool().name;
        batteryAmount = player.ActiveTool().battery;
    }
    else
    {
        activeToolName = "Niciun obiect";
        batteryAmount = 0.0f;
    }
}

void RaylibRenderer::DrawGameHUD()
{
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawRectangle(24, 24, 360, 116, RaylibColor{8, 10, 12, 205});
    DrawText(currentLocationName.empty() ? "GONJIAM" : currentLocationName.c_str(),
             42, 38, 24, RaylibColor{219, 211, 190, 255});
    DrawText(TextFormat("Frica: %d%%", static_cast<int>(fearAmount)), 42, 78, 18,
             RaylibColor{205, 126, 112, 255});
    DrawRectangle(112, 82, 105, 12, RaylibColor{53, 38, 37, 255});
    DrawRectangle(112, 82, static_cast<int>(105.0f * fearAmount / 100.0f), 12,
                  RaylibColor{197, 76, 65, 255});
    DrawText(TextFormat("Stamina: %d%%", static_cast<int>(staminaAmount)), 232, 78, 18,
             RaylibColor{160, 192, 154, 255});

    DrawRectangle(screenWidth / 2 - 7, screenHeight / 2 - 1, 14, 2,
                  RaylibColor{225, 219, 194, 210});
    DrawRectangle(screenWidth / 2 - 1, screenHeight / 2 - 7, 2, 14,
                  RaylibColor{225, 219, 194, 210});

    DrawRectangle(24, screenHeight - 108, 570, 82, RaylibColor{8, 10, 12, 205});
    DrawText(TextFormat("Unealta: %s  |  Baterie: %d%%", activeToolName.c_str(),
                        static_cast<int>(batteryAmount)),
             40, screenHeight - 96, 19, RaylibColor{219, 211, 190, 255});
    DrawText("WASD miscare  SHIFT sprint  MOUSE privire  E interactiune  CLICK foloseste",
             40, screenHeight - 67, 15, RaylibColor{172, 171, 162, 255});
    DrawText("F lanterna  Q/R schimba unealta  CLICK pe betele selectate: pune/ridica  ESC pauza",
             40, screenHeight - 44, 15, RaylibColor{172, 171, 162, 255});

    if (flashlightEnabled)
        DrawText("LANTERNA APRINSA", screenWidth - 250, 36, 20,
                 RaylibColor{223, 201, 125, 255});
    else
        DrawText("LANTERNA STINSA", screenWidth - 250, 36, 20,
                 RaylibColor{143, 143, 137, 255});
    if (rodsPlaced)
        DrawText("BETELE SUNT PLASATE", screenWidth - 250, 66, 18,
                 RaylibColor{200, 178, 116, 255});

    if (!currentSubtitle.empty())
    {
        const int textWidth = MeasureText(currentSubtitle.c_str(), 22);
        DrawRectangle(screenWidth / 2 - textWidth / 2 - 20, screenHeight - 175,
                      textWidth + 40, 46, RaylibColor{8, 10, 12, 225});
        DrawText(currentSubtitle.c_str(), screenWidth / 2 - textWidth / 2,
                 screenHeight - 162, 22, RaylibColor{226, 218, 198, 255});
    }
    if (showFPS)
        DrawFPS(screenWidth - 100, screenHeight - 35);
    if (cameraFlashTimer > 0.0f)
    {
        const unsigned char alpha = static_cast<unsigned char>(
            235.0f * std::clamp(cameraFlashTimer / 0.45f, 0.0f, 1.0f));
        DrawRectangle(0, 0, screenWidth, screenHeight,
                      RaylibColor{255, 255, 255, alpha});
    }
}

void RaylibRenderer::DrawMenuBackground(const char *heading, const char *subtitle)
{
    const int width = GetScreenWidth();
    const int height = GetScreenHeight();
    ClearBackground(RaylibColor{8, 10, 13, 255});
    DrawRectangleGradientV(0, 0, width, height, RaylibColor{13, 18, 19, 255},
                           RaylibColor{6, 7, 10, 255});
    DrawCircleGradient(width * 3 / 4, height / 2, static_cast<float>(height) * 0.56f,
                       RaylibColor{54, 43, 38, 100}, RaylibColor{9, 10, 12, 0});
    DrawRectangle(0, 0, width, height, RaylibColor{0, 0, 0, 105});
    DrawText("GONJIAM", 90, height / 4, 28, RaylibColor{176, 157, 125, 255});
    DrawText(heading, 86, height / 4 + 56, 58, RaylibColor{229, 222, 207, 255});
    DrawText(subtitle, 92, height / 4 + 126, 22, RaylibColor{158, 158, 153, 255});
}

MenuAction RaylibRenderer::DrawButton(const char *label, Rectangle bounds, MenuAction action)
{
    const Vector2 mouse = GetMousePosition();
    const bool hovered = CheckCollisionPointRec(mouse, bounds);
    DrawRectangleRec(bounds, hovered ? RaylibColor{112, 83, 61, 245}
                                     : RaylibColor{35, 38, 39, 235});
    DrawRectangleLinesEx(bounds, 1.5f,
                         hovered ? RaylibColor{205, 174, 127, 255}
                                 : RaylibColor{101, 96, 84, 255});
    const int textWidth = MeasureText(label, 22);
    DrawText(label, static_cast<int>(bounds.x + (bounds.width - textWidth) * 0.5f),
             static_cast<int>(bounds.y + 17.0f), 22, RaylibColor{229, 222, 207, 255});
    const bool clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
                         IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    return hovered && clicked ? action : MenuAction::None;
}

MenuAction RaylibRenderer::DrawMainMenu(bool canContinue)
{
    EnableCursor();
    BeginDrawing();
    DrawMenuBackground("SPITALUL PARASIT", "Exploreaza. Cauta dovezi. Nu ramane in intuneric.");
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
    {
        EndDrawing();
        return canContinue ? MenuAction::Continue : MenuAction::Start;
    }
    if (IsKeyPressed(KEY_ESCAPE))
    {
        EndDrawing();
        return MenuAction::Quit;
    }

    const float x = 92.0f;
    const float y = static_cast<float>(GetScreenHeight()) * 0.58f;
    const Rectangle button{x, y, 330.0f, 58.0f};

    MenuAction action = MenuAction::None;
    if (canContinue)
    {
        action = DrawButton("CONTINUA", button, MenuAction::Continue);
        if (action == MenuAction::None)
            action = DrawButton("INCEPE DIN NOU", Rectangle{x, y + 72.0f, 330.0f, 58.0f},
                                MenuAction::Start);
        if (action == MenuAction::None)
            action = DrawButton("IESI DIN JOC", Rectangle{x, y + 144.0f, 330.0f, 58.0f},
                                MenuAction::Quit);
    }
    else
    {
        action = DrawButton("INCEPE JOCUL", button, MenuAction::Start);
        if (action == MenuAction::None)
            action = DrawButton("IESI DIN JOC", Rectangle{x, y + 72.0f, 330.0f, 58.0f},
                                MenuAction::Quit);
    }
    DrawText("WASD pentru miscare  |  E interactiune  |  ESC pauza",
             92, GetScreenHeight() - 62, 18, RaylibColor{147, 147, 143, 255});
    DrawText("ENTER pentru a incepe / continua  |  ESC pentru iesire",
             92, GetScreenHeight() - 36, 18, RaylibColor{147, 147, 143, 255});
    EndDrawing();
    return action;
}

MenuAction RaylibRenderer::DrawPauseMenu()
{
    EnableCursor();
    BeginDrawing();
    DrawMenuBackground("PAUZA", "Te-ai oprit. Intunericul nu.");
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
    {
        EndDrawing();
        return MenuAction::Resume;
    }
    if (IsKeyPressed(KEY_M))
    {
        EndDrawing();
        return MenuAction::MainMenu;
    }
    if (IsKeyPressed(KEY_Q))
    {
        EndDrawing();
        return MenuAction::Quit;
    }
    const float x = 92.0f;
    const float y = static_cast<float>(GetScreenHeight()) * 0.56f;
    MenuAction action = DrawButton("CONTINUA EXPLORAREA",
                                  Rectangle{x, y, 370.0f, 58.0f}, MenuAction::Resume);
    if (action == MenuAction::None)
        action = DrawButton("MENIU PRINCIPAL",
                            Rectangle{x, y + 72.0f, 370.0f, 58.0f}, MenuAction::MainMenu);
    if (action == MenuAction::None)
        action = DrawButton("IESI DIN JOC",
                            Rectangle{x, y + 144.0f, 370.0f, 58.0f}, MenuAction::Quit);
    DrawText("ENTER: continua  |  M: meniu principal  |  Q: iesire",
             92, GetScreenHeight() - 42, 18, RaylibColor{147, 147, 143, 255});
    EndDrawing();
    return action;
}
