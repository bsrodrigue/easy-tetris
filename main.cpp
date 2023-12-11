#include <cstdlib>
#include <raylib.h>
#include <raymath.h>

//------------ TETRIS -----------------//

//--------- WINDOW
#define WINDOW_TITLE "Easy Tetris"
#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 480

//--------- PERFORMANCE
#define TARGET_FPS 30

//--------- CELLS
#define CELL_SIZE 20
#define CELL_COUNT_VERTICAL WINDOW_HEIGHT / CELL_SIZE
#define CELL_COUNT_HORIZONTAL WINDOW_WIDTH / CELL_SIZE

#define CELL_COUNT 4

using namespace std;

namespace tetris {

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

Block create_block() {
  Block block;

  for (int i = 0; i < CELL_COUNT; i++) {
    // Init cells to -1
    block.body[i].x = -1;
    block.body[i].y = -1;
  }

  Vector2 first_cell = get_random_vec2();

  block.body[0] = first_cell;

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

void init() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
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

int main(int argc, char *argv[]) {
  tetris::init();

  tetris::Block block1 = tetris::create_block();
  tetris::Block block2 = tetris::create_block();
  tetris::Block block3 = tetris::create_block();
  tetris::Block block4 = tetris::create_block();

  // Game loop
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    tetris::draw_block(block1);
    tetris::draw_block(block2);
    tetris::draw_block(block3);
    tetris::draw_block(block4);

    EndDrawing();
  }

  tetris::close();
  return EXIT_SUCCESS;
}
