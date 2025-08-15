#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"
#include "raylib.h"
#define GRID_MULT 32
#define GRID_SIZE (1 * GRID_MULT)
#define CELL_SIZE (960 / GRID_MULT)
#define WINDOW_WIDTH (GRID_SIZE * CELL_SIZE)
#define WINDOW_HEIGHT (GRID_SIZE * CELL_SIZE + 40)


#define SIMULATION_SPEED 30.0f

typedef struct {
    Rectangle rect;
    Color color;

    bool selected;
    bool active;
} Cell;

typedef struct {
    Cell* items;
    size_t capacity, count;

    bool started;
    float frameCounter;
} Grid;

#define LIGHTCOLOR RAYWHITE
#define DARKCOLOR LIGHTGRAY
#define GRIDLINECOLOR BLACK
#define GRIDLINETICKNESS 1.0f
#define MOUSEHITBOX 0.1f

int countNeighbors(Grid* grid, int col, int row){
    int liveNeighbors = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;

            int neighborRow = col + i;
            int neighborCol = row + j;

            if (neighborRow >= 0 && neighborRow < GRID_SIZE && neighborCol >= 0 && neighborCol < GRID_SIZE) {
                liveNeighbors += grid->items[neighborRow * GRID_SIZE + neighborCol].active ? 1 : 0;
            }
        }
    }
    return liveNeighbors;
}

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game of life");

    SetTargetFPS(60);

    Rectangle mouse_rect = (Rectangle) {
        .width = MOUSEHITBOX,
        .height = MOUSEHITBOX,
    };
    Grid grid = {0};
    char * legend = temp_sprintf("%dx%d Grid", GRID_SIZE, GRID_SIZE);
    const int cell_size = CELL_SIZE;

    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            Cell cell = (Cell) {
                .rect = (Rectangle) {
                    .x = col * cell_size,
                    .y = row * cell_size + 30,
                    .width = cell_size,
                    .height = cell_size,
                },
                .color = ((row + col) % 2 == 0) ? LIGHTCOLOR : DARKCOLOR,
            };
            da_append(&grid, cell);
        }
    }

    while (!WindowShouldClose()) {
        if (grid.started) {

            grid.frameCounter++;

            if (grid.frameCounter >= SIMULATION_SPEED) {
                grid.frameCounter = 0;

                da_foreach(Cell, it, &grid) {
                    int col = it->rect.x / cell_size;
                    int row = (it->rect.y - 30) / cell_size;
                    int n = countNeighbors(&grid, row, col);
                    if (it->active) {
                        if (n < 2 || n > 3) {
                            it->active = false;
                        } else {
                            it->active = true;
                        }
                    } else {
                        if (n == 3) {
                            it->active = true;
                        } else {
                            it->active = false;
                        }
                    }
                }
            }

            if(IsKeyPressed(KEY_SPACE)) {
                grid.started = false;
                grid.frameCounter = 0;
            }
        } else {
            da_foreach(Cell, it, &grid) {
                Vector2 mouse_pos = GetMousePosition();
                mouse_rect.x = mouse_pos.x;
                mouse_rect.y = mouse_pos.y;
                if(CheckCollisionRecs(it->rect, mouse_rect)) {
                    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                        it->active = true;
                    } else {
                        it->selected = true;
                    }
                } else {
                    it->selected = false;
                }
            }
            if(IsKeyPressed(KEY_SPACE)) {
                grid.started = true;
                grid.frameCounter = 0;
            }
        }

        BeginDrawing();

        ClearBackground(DARKGRAY);

        DrawText(legend, 10, 10, 20, WHITE);

        da_foreach(Cell, it, &grid) {
            Color c;
            if (it->active) {
                c = BLACK;
            } else if (it->selected) {
                if (grid.started) it->selected=false;
                else c = ColorBrightness(RED, 0.3);
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
