#include <algorithm>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>

//--------------------- TETRIS -----------------------------//

//--------- WINDOW
#define WINDOW_TITLE "Easy Tetris"
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 400

//--------- PERFORMANCE
#define TARGET_FPS 30

//--------- CELLS
#define CELL_COUNT_HORIZONTAL 10
#define CELL_COUNT_VERTICAL 20
#define CELL_SIZE WINDOW_WIDTH / CELL_COUNT_HORIZONTAL
#define AUTO_WINDOW_HEIGHT CELL_SIZE *CELL_COUNT_VERTICAL

#define CELL_COUNT 4

using namespace std;

namespace tetris {

void draw_grid() {
  // Horizontal
  for (int i = 0; i < CELL_COUNT_VERTICAL; i++) {
    DrawLine(0, i * CELL_SIZE, WINDOW_WIDTH, i * CELL_SIZE, WHITE);
  }

  // Vertical
  for (int i = 0; i < CELL_COUNT_HORIZONTAL; i++) {
    DrawLine(i * CELL_SIZE, 0, i * CELL_SIZE, AUTO_WINDOW_HEIGHT, WHITE);
  }
}

Vector2 get_random_vec2();
void draw_cell(int x, int y, Color color);

enum CellDirection {
  UP,
  RIGHT,
  DOWN,
  LEFT,
};

// A block is made of one or more cells
typedef struct {
  Vector2 body[CELL_COUNT];
} Block;

bool cell_exists(Block block, Vector2 pos) {
  for (int i = 0; i < CELL_COUNT; i++) {
    if (Vector2Equals(block.body[i], pos))
      return true;
  }

  return false;
}

Block create_block(Vector2 initial_pos) {
  Block block;

  for (int i = 0; i < CELL_COUNT; i++) {
    // Init cells to -1
    block.body[i].x = -1;
    block.body[i].y = -1;
  }

  block.body[0] = initial_pos;

  for (int i = 1; i < CELL_COUNT; i++) {

    while (true) {
      int direction = GetRandomValue(0, 3);
      int cell = GetRandomValue(0, i - 1);

      Vector2 inplace = block.body[cell];

      if (direction == UP) {
        if (cell_exists(block, Vector2Add(inplace, {0, -1})))
          continue;
        block.body[i].x = inplace.x;
        block.body[i].y = inplace.y - 1;
      }

      else if (direction == RIGHT) {
        if (cell_exists(block, Vector2Add(inplace, {1, 0})))
          continue;
        block.body[i].x = inplace.x + 1;
        block.body[i].y = inplace.y;
      }

      else if (direction == DOWN) {
        if (cell_exists(block, Vector2Add(inplace, {0, 1})))
          continue;
        block.body[i].x = inplace.x;
        block.body[i].y = inplace.y + 1;
      }

      else if (direction == LEFT) {
        if (cell_exists(block, Vector2Add(inplace, {-1, 0})))
          continue;
        block.body[i].x = inplace.x - 1;
        block.body[i].y = inplace.y;
      }

      break;
    }
  }

  return block;
}

void draw_block(Block block) {
  for (int i = 0; i < CELL_COUNT; i++) {
    if (block.body[i].x == -1)
      return;

    draw_cell(block.body[i].x, block.body[i].y, WHITE);
  }
}

void fall(Block *block) {
  for (int i = 0; i < CELL_COUNT; i++) {
    block->body[i] = Vector2Add(block->body[i], {0, 1});
  }
}

void move(Block *block, Vector2 vec2) {
  for (int i = 0; i < CELL_COUNT; i++) {
    block->body[i] = Vector2Add(block->body[i], vec2);
  }
}

void handle_input(Block *block) {
  int key = GetKeyPressed();

  switch (key) {
  case KEY_LEFT:
    move(block, {-1, 0});
    break;
  case KEY_RIGHT:
    move(block, {1, 0});
    break;
  }
}

void init() {
  InitWindow(WINDOW_WIDTH, AUTO_WINDOW_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(TARGET_FPS);
}

Vector2 get_random_vec2() {
  float x = GetRandomValue(0, CELL_COUNT_HORIZONTAL - 1);
  float y = GetRandomValue(0, CELL_COUNT_VERTICAL - 1);

  return {x, y};
}

void draw_cell(int x, int y, Color color) {
  DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}

void print_fps() { DrawFPS(10, 10); }

void close() { CloseWindow(); }
}; // namespace tetris

double last_updatetime = 0;

bool event_triggered(double interval) {
  double current_time = GetTime();

  if (current_time - last_updatetime >= interval) {
    last_updatetime = current_time;
    return true;
  }

  return false;
}

int main(int argc, char *argv[]) {
  tetris::init();
  // Vector2 first_cell = get_random_vec2();

  tetris::Block block1 = tetris::create_block({3, 3});

  // Game loop
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    tetris::draw_grid();
    tetris::handle_input(&block1);

    if (event_triggered(1)) {
      tetris::fall(&block1);
    }

    tetris::draw_block(block1);

    EndDrawing();
  }

  tetris::close();
  return EXIT_SUCCESS;
}
