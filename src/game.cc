#include "include/game.h"
#include "include/constants.h"
#include "include/entities.h"
#include "include/entity.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

Camera2D camera;
EntityManager entities;

void Game::Update(float dt) {
    entities.CleanupRemoved();
    UpdateState(dt);
    ManageState();
}

void Game::Draw() { DrawState(); }

void Game::ManageState() {
    switch (GameState) {
    case TITLE:

        if (IsKeyPressed(KEY_ENTER)) {
            GameState = LEVEL;
        }
        break;
    case LEVEL:

        if (IsKeyPressed(KEY_ENTER)) {
            GameState = EDITOR;
        }
        break;
    case EDITOR:

        if (IsKeyPressed(KEY_ENTER)) {
            GameState = LEVEL;
        }
        camera.zoom = cameraZoom;
        camera.target = cameraTarg;
        break;
    default:
        break;
    }
}
void Game::UpdateState(float dt) {
    switch (GameState) {
    case TITLE:
        break;
    case LEVEL:
        entities.UpdateAll(dt);
        break;
    case EDITOR:
        EditLevel();
        break;
    default:
        break;
    }
}
void Game::DrawState() {
    switch (GameState) {
    case TITLE:
        DrawText("hello", 75, 75, 15, BLACK);

        break;
    case LEVEL:
        BeginMode2D(camera);
        entities.DrawAll();

        DrawCircle(50, 50, 50, BLACK);

        EndMode2D();

        DrawText("hello", 75, 75, 15, BLUE);
        break;
    case EDITOR:
        BeginMode2D(camera);
        entities.DrawAll();

        DrawGrid();
        DrawRectangleLinesEx(removeRect, 1.0f, BLACK);
        EndMode2D();

        DrawText("hello", 75, 75, 15, GREEN);
        break;
    default:
        break;
    }
}

void Game::DrawGrid() {
    Vector2 topLeft = GetScreenToWorld2D({0, 0}, camera);
    Vector2 bottomRight = GetScreenToWorld2D(
        {(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);

    int startX = floor(topLeft.x / GRID_SIZE) * GRID_SIZE;
    int startY = floor(topLeft.y / GRID_SIZE) * GRID_SIZE;
    int endX = ceil(bottomRight.x / GRID_SIZE) *
               GRID_SIZE; // Ensure grid covers full screen
    int endY = ceil(bottomRight.y / GRID_SIZE) * GRID_SIZE;

    // OPTIMIZATION: Manual Line Batching
    // Instead of hundreds of DrawLine calls, we send all vertices to the GPU at
    // once.
    rlBegin(RL_LINES);
    rlColor4ub(160, 160, 160, 255); // GRAY
    for (int x = startX; x <= endX; x += GRID_SIZE) {
        rlVertex2f((float)x, (float)startY);
        rlVertex2f((float)x, (float)endY);
    }
    for (int y = startY; y <= endY; y += GRID_SIZE) {
        rlVertex2f((float)startX, (float)y);
        rlVertex2f((float)endX, (float)y);
    }
    rlEnd();
}

void Game::EditLevel() {
    float zoomS = 0.1f;
    float moveS = 5.0f;
    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
    float wheel = GetMouseWheelMove();

    // Snap to grid
    Vector2 snapped;
    snapped.x = floor(mousePos.x / GRID_SIZE) * GRID_SIZE + GRID_SIZE / 2.0f;
    snapped.y = floor(mousePos.y / GRID_SIZE) * GRID_SIZE + GRID_SIZE / 2.0f;

    // Camera zoom
    camera.zoom = cameraZoom;
    camera.target = cameraTarg;
    cameraZoom = std::clamp(cameraZoom + wheel * 0.1f, 0.2f, 5.0f);

    // Camera Controls
    if (IsKeyDown(KEY_MOVE_UP))
        cameraTarg.y -= moveS;
    else if (IsKeyDown(KEY_MOVE_DOWN))
        cameraTarg.y += moveS;
    if (IsKeyDown(KEY_MOVE_LEFT))
        cameraTarg.x -= moveS;
    else if (IsKeyDown(KEY_MOVE_RIGHT))
        cameraTarg.x += moveS;

    // Tool rectangle
    removeRectSize = {EToolSize * GRID_SIZE, EToolSize * GRID_SIZE};
    removeRect = {
        snapped.x - removeRectSize.x / 2,
        snapped.y - removeRectSize.y / 2,
        removeRectSize.x,
    };

    if (IsKeyPressed(KEY_NEXT_TOOL_NUM))
        EToolNum++;
    if (IsKeyPressed(KEY_LAST_TOOL_NUM))
        EToolNum--;
    if (IsKeyPressed(KEY_NEXT_TOOL_SIZE))
        EToolSize++;
    if (IsKeyPressed(KEY_LAST_TOOL_SIZE))
        EToolSize--;
}

void Game::SpawnEntity() {}
