#include "ZureLib/zurelib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

ZURELIB_WHDLE window;

int window_width = 800;
int window_height = 600;

// Player properties
#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 50
float player_x, player_y;
float player_velocity_y;
#define PLAYER_JUMP_FORCE -10.0f
#define GRAVITY 0.3f

// Platform properties
#define PLATFORM_COUNT 10
#define PLATFORM_WIDTH 100
#define PLATFORM_HEIGHT 20
float platform_x[PLATFORM_COUNT], platform_y[PLATFORM_COUNT];

// Game properties
int score = 0;
int game_running = 1;
float camera_offset_y = 0;

// Function to initialize platforms at random positions
void initialize_platforms() {
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        platform_x[i] = rand() % (window_width - PLATFORM_WIDTH);
        platform_y[i] = window_height - (i * (window_height / PLATFORM_COUNT));
    }
}

// Function to reset the game
void initialize_game() {
    player_x = window_width / 2 - PLAYER_WIDTH / 2;
    player_y = window_height - PLAYER_HEIGHT * 3;
    //forcibly place platform under player at start
    player_y = platform_y[0] - PLAYER_HEIGHT;
    player_velocity_y = 0;
    score = 0;
    camera_offset_y = 0;
    initialize_platforms();
}

void on_window_resize(unsigned int width, unsigned int height) {
    window_width = width;
    window_height = height;
}

void on_window_close() {
    zl_qdestroy_window(window);
    zl_deinit();
    exit(0);
}

// Draw the game objects
void draw_game() {
    zl_qclear_window(window);
    
    // Draw the background
    zl_qdraw_filled_rect(window, 0, 0, window_width, window_height, 0x87CEEB); // Sky blue

    // Draw the player
    zl_qdraw_filled_rect(window, player_x, player_y - camera_offset_y, PLAYER_WIDTH, PLAYER_HEIGHT, 0xFF6347); // Tomato red

    // Draw the platforms
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        zl_qdraw_filled_rect(window, platform_x[i], platform_y[i] - camera_offset_y, PLATFORM_WIDTH, PLATFORM_HEIGHT, 0x32CD32); // Lime green
    }

    // Draw the score
    char score_text[32];
    sprintf(score_text, "Score: %d", score);
    zl_qdraw_string(window, 10, 10, score_text, 0xFFFFFF);
}

// Update game logic
void update_game() {
    // Apply gravity
    player_velocity_y += GRAVITY * ((score/100) == 0 ? 1 : (score/100));
    player_y += player_velocity_y;

    // Check for collision with platforms
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        if (player_x + PLAYER_WIDTH > platform_x[i] && player_x < platform_x[i] + PLATFORM_WIDTH &&
            player_y + PLAYER_HEIGHT > platform_y[i] && player_y + PLAYER_HEIGHT - player_velocity_y < platform_y[i]) {
            player_velocity_y = PLAYER_JUMP_FORCE;
            score++;
        }
    }
    // Handle keyboard input
    if (zl_qkey_down(ZL_KEY_LEFT)) {
        player_x -= 10;
        if (player_x < 0) player_x = 0;
    }
    if (zl_qkey_down(ZL_KEY_RIGHT)) {
        player_x += 10;
        if (player_x > window_width - PLAYER_WIDTH) player_x = window_width - PLAYER_WIDTH;
    }
    if (zl_qkey_down(ZL_KEY_PLUS)) {
        player_velocity_y = PLAYER_JUMP_FORCE;
    }

    // Scroll the camera upward if the player jumps higher
    if (player_y - camera_offset_y < window_height / 2) {
        camera_offset_y = player_y - window_height / 2;
    }

    // Recycle platforms that go off-screen
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        if (platform_y[i] - camera_offset_y > window_height) {
            platform_x[i] = rand() % (window_width - PLATFORM_WIDTH);
            platform_y[i] -= window_height;
        }
    }

    // End the game if the player falls below the screen
    if (player_y - camera_offset_y > window_height) {
        initialize_game();
    }
}

// Handle keyboard input
void handle_input() {
    if (zl_qkey_down(ZL_KEY_LEFT)) {
        player_x -= 10;
        if (player_x < 0) player_x = 0;
    }
    if (zl_qkey_down(ZL_KEY_RIGHT)) {
        player_x += 10;
        if (player_x > window_width - PLAYER_WIDTH) player_x = window_width - PLAYER_WIDTH;
    }
}

int main() {
    zl_init(ZL_TRUE);
    zl_qset_console_visibility(0);
    srand((unsigned int)time(NULL));
    window = zl_qcreate_window("Doodle Jump Clone", window_width, window_height);

    if (!window) {
        zl_qwriteln("Failed to create window!");
        zl_deinit();
        return 1;
    }

    zl_register_on_resize(on_window_resize);
    zl_register_on_close(on_window_close);

    initialize_game();

    while (game_running && !zl_qshould_window_close(window)) {
        zl_qdo_events(window);

        // Handle player input
        handle_input();

        // Update game logic
        update_game();

        // Draw the game
        draw_game();

        zl_qupdate_window(window);

        // Cap the framerate to 60fps
        zl_qsleep(16);
    }

    zl_qdestroy_window(window);
    zl_deinit();
    return 0;
}
