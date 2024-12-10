#include "ZureLib/zurelib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h> // For random food placement

ZURELIB_WHDLE window;

int window_width = 800;
int window_height = 600;

// Snake properties
#define GRID_SIZE 20
int snake_length = 1;
int snake_x[100], snake_y[100]; // Maximum snake length: 100 segments
int direction_x = 1, direction_y = 0; // Initial direction: right

// Food properties
int food_x, food_y;

// Game properties
int game_running = 1;
int score = 0;
double move_timer = 0.1; // Time between snake movements
double time_elapsed = 0;

// Function to generate random food location
void place_food() {
    food_x = (rand() % (window_width / GRID_SIZE)) * GRID_SIZE;
    food_y = (rand() % (window_height / GRID_SIZE)) * GRID_SIZE;
}

// Function to initialize the snake and food
void initialize_game() {
    snake_x[0] = GRID_SIZE * 5; // Start position
    snake_y[0] = GRID_SIZE * 5;
    snake_length = 1;
    direction_x = 1;
    direction_y = 0;
    score = 0;
    place_food();
}

void on_window_resize(unsigned int width, unsigned int height) {
    window_width = width;
    window_height = height;
    place_food();
}

void on_window_close() {
    zl_qdestroy_window(window);
    zl_deinit();
    exit(0);
}

// Draw score on the screen
void draw_score(ZL_TTF_FONT fnt) {
    char score_text[32];
    sprintf(score_text, "Score: %d", score);
    if (fnt.font) {
        zl_qdraw_string_font(window, 10, 10, score_text, 0xFFFFFF, fnt);
    } else {
        zl_qdraw_string(window, 10, 10, score_text, 0xFFFFFF);
    }
}

// Draw the game screen
void draw_game() {
    zl_qclear_window(window);

    // Draw the background
    zl_qdraw_rect(window, 0, 0, window_width, window_height, 0x000000);

    // Draw the snake
    for (int i = 0; i < snake_length; i++) {
        zl_qdraw_rect(window, snake_x[i], snake_y[i], GRID_SIZE, GRID_SIZE, 0x00FF00);
    }

    // Draw the food
    zl_qdraw_rect(window, food_x, food_y, GRID_SIZE, GRID_SIZE, 0xFF0000);
}

// Move the snake and handle game logic
void update_snake() {
    // Update the position of the snake segments
    for (int i = snake_length - 1; i > 0; i--) {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    // Move the head of the snake
    snake_x[0] += direction_x * GRID_SIZE;
    snake_y[0] += direction_y * GRID_SIZE;

    // Check for collision with food
    if (snake_x[0] == food_x && snake_y[0] == food_y) {
        snake_length++;
        score++;
        place_food();
    }

    // Check for collision with walls
    if (snake_x[0] < 0 || snake_x[0] >= window_width ||
        snake_y[0] < 0 || snake_y[0] >= window_height) {
        game_running = 0;
    }

    // Check for collision with itself
    for (int i = 1; i < snake_length; i++) {
        if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) {
            game_running = 0;
            break;
        }
    }
}

// Handle keyboard input for snake direction
void handle_input() {
    if (zl_qkey_down(ZL_KEY_UP) && direction_y == 0) {
        direction_x = 0;
        direction_y = -1;
    }
    if (zl_qkey_down(ZL_KEY_DOWN) && direction_y == 0) {
        direction_x = 0;
        direction_y = 1;
    }
    if (zl_qkey_down(ZL_KEY_LEFT) && direction_x == 0) {
        direction_x = -1;
        direction_y = 0;
    }
    if (zl_qkey_down(ZL_KEY_RIGHT) && direction_x == 0) {
        direction_x = 1;
        direction_y = 0;
    }
}

int main() {
    zl_init(ZL_TRUE);
    zl_qset_console_visibility(0);
    srand((unsigned int)time(NULL)); // Seed for randomization
    window = zl_qcreate_window("Snake Game", window_width, window_height);

    if (!window) {
        zl_qwriteln("Failed to create window!");
        zl_deinit();
        return 1;
    }

    zl_register_on_resize(on_window_resize);
    zl_register_on_close(on_window_close);

    initialize_game();

    double last_time = zl_get_time();

    while (game_running && !zl_qshould_window_close(window)) {
        zl_qdo_events(window);

        // Handle player input
        handle_input();

        // Update game logic based on timer
        double current_time = zl_get_time();
        time_elapsed += current_time - last_time;
        last_time = current_time;

        if (time_elapsed >= move_timer) {
            time_elapsed -= move_timer;
            update_snake();
        }

        // Draw the game
        draw_game();
        draw_score((ZL_TTF_FONT) {0});

        zl_qupdate_window(window);

        // Cap the framerate to 60fps
        double frame_time = zl_get_time() - current_time;
        if (frame_time < 1.0 / 60.0) {
            zl_qsleep((unsigned int)((1.0 / 60.0 - frame_time) * 1000));
        }
    }

    zl_qdestroy_window(window);
    zl_deinit();
    return 0;
}
