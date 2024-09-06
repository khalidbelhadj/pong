#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

#define WIDTH 1920
#define HEIGHT 1080
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

#define P_WIDTH 10
#define P_HEIGHT 100
#define RATE 1.05

struct {
    Rectangle p1_rect;
    size_t p1_score;

    Rectangle p2_rect;
    size_t p2_score;

    Vector2 b_pos;
    Vector2 b_dir;
    float b_speed;

    bool first;
} state = {0};

double rad(int degrees) { return PI * (degrees / 180.); }

Vector2 f(int angle, int speed) {
    return (Vector2){
        cos(rad(angle)) * speed,
        sin(rad(angle)) * speed,
    };
}

void reset_ball(void) {
    static int a = 1;
    state.b_speed = 8;
    state.b_pos = (Vector2){WIDTH / 2., HEIGHT / 2.};
    state.b_dir = (Vector2){a * state.b_speed / 2., 0};
    state.first = true;
    a *= -1;
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, "Pong");
    SetTargetFPS(60);
    MaximizeWindow();

    reset_ball();

    state.p1_rect = (Rectangle){5, 5, P_WIDTH, P_HEIGHT};
    state.p2_rect = (Rectangle){WIDTH - P_WIDTH - 5, 5, P_WIDTH, P_HEIGHT};
    state.first = true;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Player 1
        DrawRectangleRec(state.p1_rect, WHITE);

        if (IsKeyDown(KEY_S)) {
            state.p1_rect.y = MIN(state.p1_rect.y + 10, HEIGHT - P_HEIGHT);
        } else if (IsKeyDown(KEY_W)) {
            state.p1_rect.y = MAX(state.p1_rect.y - 10, 0);
        }

        // Player 2
        DrawRectangleRec(state.p2_rect, WHITE);

        if (IsKeyDown(KEY_DOWN)) {
            state.p2_rect.y = MIN(state.p2_rect.y + 10, HEIGHT - P_HEIGHT);
        } else if (IsKeyDown(KEY_UP)) {
            state.p2_rect.y = MAX(state.p2_rect.y - 10, 0);
        }

        DrawLine(WIDTH / 2., 0, WIDTH / 2., HEIGHT, WHITE);

        // Score
        const char *text;

        text = TextFormat("%d", state.p1_score);
        DrawText(text, WIDTH / 2. - MeasureText(text, 50) - 10, 10, 50, WHITE);

        text = TextFormat("%d", state.p2_score);
        DrawText(text, WIDTH / 2. + 10, 10, 50, WHITE);

        // Ball and player collision
        if (CheckCollisionCircleRec(state.b_pos, 8, state.p1_rect)) {
            if (state.first) {
                state.first = false;
                state.b_dir = Vector2Scale(state.b_dir, 2);
            }

            state.b_speed = MIN(RATE * state.b_speed, 15);
            float frac =
                (state.b_pos.y - state.p1_rect.y) / state.p1_rect.height;
            float theta = (PI / 2) * frac - PI / 4;
            state.b_dir = (Vector2){state.b_speed * cos(theta),
                                    state.b_speed * sin(theta)};
        }

        if (CheckCollisionCircleRec(state.b_pos, 8, state.p2_rect)) {
            if (state.first) {
                state.first = false;
                state.b_dir = Vector2Scale(state.b_dir, 2);
            }

            state.b_speed = MIN(RATE * state.b_speed, 15);
            float frac =
                1 - (state.b_pos.y - state.p2_rect.y) / state.p2_rect.height;
            float theta = (PI / 2) * frac + 3 * PI / 4;
            state.b_dir = (Vector2){state.b_speed * cos(theta),
                                    state.b_speed * sin(theta)};
        }

        // Ball and border collision
        if (state.b_pos.y <= 0 || state.b_pos.y >= HEIGHT) {
            state.b_dir.y *= -1;
        }

        if (state.b_pos.x <= 0) {
            ++state.p2_score;
            reset_ball();
            continue;
        }

        if (state.b_pos.x >= WIDTH) {
            ++state.p1_score;
            reset_ball();
            continue;
        }

        state.b_pos = Vector2Add(state.b_pos, state.b_dir);
        DrawCircleV(state.b_pos, 8, WHITE);

        EndDrawing();
    }

    CloseWindow();
}
