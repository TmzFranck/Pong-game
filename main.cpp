#include <raylib.h>

constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 800;
Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 154, 255};

int player_score = 0;
int cpu_score = 0;

struct Ball {
  float x, y;
  int speed_x, speed_y;
  int radius;

  void draw() { DrawCircle(x, y, radius, Yellow); }

  void update() {
    x += speed_x;
    y += speed_y;

    if (y + radius >= GetScreenHeight() || y - radius <= 0) {
      speed_y *= -1;
    }
    if (x - radius <= 0) {
      player_score += 1;
      reset();
    }
    if (x + radius >= GetScreenWidth()) {
      cpu_score += 1;
      reset();
    }
  }

  void reset() {
    x = (float)GetScreenWidth() / 2;
    y = (float)GetScreenHeight() / 2;

    int speed_choices[2] = {-1, 1};
    speed_x *= speed_choices[GetRandomValue(0, 1)];
    speed_y *= speed_choices[GetRandomValue(0, 1)];
  }
};

struct Paddle {
  float x, y;
  float height, width;
  int speed;

  void limit_move() {
    if (y <= 0) {
      y = 0;
    } else if (y + height >= GetScreenHeight()) {
      y = GetScreenHeight() - height;
    }
  }

  void draw() {
    DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
  }

  void update() {
    if (IsKeyDown(KEY_UP)) {
      y -= speed;
    } else if (IsKeyDown(KEY_DOWN)) {
      y += speed;
    }
    limit_move();
  }
};

struct CpuPadle : public Paddle {
  void update(int ball_y) {
    if (y + height / 2 > ball_y) {
      y -= speed;
    } else if (y + height / 2 < ball_y) {
      y += speed;
    }
    limit_move();
  }
};

int main() {

  Ball ball{.x = (float)SCREEN_WIDTH / 2,
            .y = (float)SCREEN_HEIGHT / 2,
            .speed_x = 7,
            .speed_y = 7,
            .radius = 20};

  Paddle player;
  player.height = 120;
  player.width = 25;
  player.speed = 6;
  player.x = (float)SCREEN_WIDTH - player.width - 10;
  player.y = (float)SCREEN_HEIGHT / 2 - player.height / 2;

  CpuPadle cpu;
  cpu.height = 120;
  cpu.width = 25;
  cpu.speed = 6;
  cpu.x = 10;
  cpu.y = (float)SCREEN_HEIGHT / 2 - cpu.height / 2;

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    // updating
    ball.update();
    player.update();
    cpu.update(ball.y);

    // check for collisions
    if (CheckCollisionCircleRec(
            Vector2{ball.x, ball.y}, ball.radius,
            Rectangle{player.x, player.y, player.width, player.height}) ||
        CheckCollisionCircleRec(
            Vector2{ball.x, ball.y}, ball.radius,
            Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
      ball.speed_x *= -1;
    }

    // Drawing
    ClearBackground(Dark_Green);
    DrawRectangle(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, Green);
    DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 150, Light_Green);
    DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);
    ball.draw();
    player.draw();
    cpu.draw();
    DrawText(TextFormat("%i", cpu_score), SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);
    DrawText(TextFormat("%i", player_score), 3 * SCREEN_WIDTH / 4 - 20, 20, 80,
             WHITE);

    EndDrawing();
  }
  CloseWindow();
}
