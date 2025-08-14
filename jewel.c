#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"
#include "raylib.h"
#define GRID_SIZE 8
#define CELL_SIZE 60
#define WINDOW_WIDTH (GRID_SIZE * CELL_SIZE)
#define WINDOW_HEIGHT (GRID_SIZE * CELL_SIZE + 40)

typedef struct {
    Rectangle rect;
    bool selected;
    Color color;
} Cell;

typedef struct {
    Cell* items;
    size_t capacity, count;
} Grid;

#define LIGHTCOLOR RAYWHITE
#define DARKCOLOR LIGHTGRAY
#define GRIDLINECOLOR BLACK
#define GRIDLINETICKNESS 1.0f
#define MOUSEHITBOX 0.1f

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "8x8 Grid Display");

    SetTargetFPS(60);

    Rectangle mouse_rect = (Rectangle) {
        .width = MOUSEHITBOX,
        .height = MOUSEHITBOX,
    };
    Grid grid = {0};

    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            int x = col * CELL_SIZE;
            int y = row * CELL_SIZE + 30;

            Color cellColor = ((row + col) % 2 == 0) ? LIGHTCOLOR : DARKCOLOR;

            Cell cell = (Cell) {
                .rect = (Rectangle) {
                    .x = x,
                    .y = y,
                    .width = CELL_SIZE,
                    .height = CELL_SIZE,
                },
                .color = cellColor
            };
            da_append(&grid, cell);
        }
    }

    while (!WindowShouldClose()) {
        da_foreach(Cell, it, &grid) {
            Vector2 mouse_pos = GetMousePosition();
            mouse_rect.x = mouse_pos.x;
            mouse_rect.y = mouse_pos.y;
            if(CheckCollisionRecs(it->rect, mouse_rect)) {
                it->selected = true;
            } else {
                it->selected = false;
            }
        }

        BeginDrawing();

        ClearBackground(DARKGRAY);

        DrawText("8x8 Grid", 10, 10, 20, WHITE);

        da_foreach(Cell, it, &grid) {
            Color c;
            if (it->selected) {
                c = RED;
            } else {
                c = it->color;
            }

            DrawRectangleRec(it->rect, c);

            DrawRectangleLinesEx(it->rect, GRIDLINETICKNESS, GRIDLINECOLOR);
        }

        DrawRectangleLines(0, 30, WINDOW_WIDTH, GRID_SIZE * CELL_SIZE, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
