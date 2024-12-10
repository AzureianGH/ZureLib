#include "ZureLib/zurelib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <time.h> // For randomization

ZURELIB_WHDLE window;

int new_window_width = 800;
int new_window_height = 600;

// Paddle and Ball properties
int paddle_width = 10;
int paddle_height = 100;
int ball_size = 10;

int player_paddle_x = 50;
int player_paddle_y = 250;
int bot_paddle_x = 750;
int bot_paddle_y = 250;
int ball_x = 400, ball_y = 300;
int ball_dx = 2, ball_dy = 2;
int player_speed = 5;
int bot_speed = 4;

int player_score = 0;
int bot_score = 0;

int bot_reaction_delay = 0; // Delay counter for the bot

void on_window_resize(unsigned int width, unsigned int height) {
    new_window_width = width;
    new_window_height = height;
}

void on_window_close() {
    zl_qdestroy_window(window);
    zl_deinit();
    exit(0);
}

void draw_score() {
    char score_text[32];
    snprintf(score_text, sizeof(score_text), "Player: %d  Bot: %d", player_score, bot_score);

    zl_qdraw_string(window, 0, 10, score_text, 0xFFFFFF);
}

void pong_game_loop() {
    srand((unsigned int)time(NULL)); // Seed for randomization

    while (!zl_qshould_window_close(window)) {
        zl_qclear_window(window);

        // Draw the black background
        zl_qdraw_rect(window, 0, 0, new_window_width, new_window_height, 0x000000);

        // Draw player paddle
        zl_qdraw_rect(window, player_paddle_x, player_paddle_y, paddle_width, paddle_height, 0xFFFFFF);

        // Draw bot paddle
        zl_qdraw_rect(window, bot_paddle_x, bot_paddle_y, paddle_width, paddle_height, 0xFFFFFF);

        // Draw ball
        zl_qdraw_rect(window, ball_x, ball_y, ball_size, ball_size, 0xFFFFFF);

        // Draw the score
        draw_score();

        // Player paddle movement
        if (zl_qkey_down(ZL_KEY_W) && player_paddle_y > 0) {
            player_paddle_y -= player_speed;
        }
        if (zl_qkey_down(ZL_KEY_S) && player_paddle_y < new_window_height - paddle_height) {
            player_paddle_y += player_speed;
        }

        // Bot paddle AI with delay and randomness
        if (bot_reaction_delay <= 0) {
            int target_position = ball_y + rand() % 21 - 10; // Add randomness
            if (bot_paddle_y + paddle_height / 2 < target_position) {
                bot_paddle_y += bot_speed;
            }
            if (bot_paddle_y + paddle_height / 2 > target_position) {
                bot_paddle_y -= bot_speed;
            }
            bot_reaction_delay = 3; // Reset delay
        } else {
            bot_reaction_delay--;
        }

        if (bot_paddle_y < 0) bot_paddle_y = 0;
        if (bot_paddle_y > new_window_height - paddle_height) bot_paddle_y = new_window_height - paddle_height;

        // Ball movement
        ball_x += ball_dx;
        ball_y += ball_dy;

        // Ball collision with top and bottom walls
        if (ball_y <= 0 || ball_y >= new_window_height - 50 - ball_size) {
            ball_dy = -ball_dy;
        }

        // Ball collision with player paddle
        if (ball_x <= player_paddle_x + paddle_width &&
            ball_y + ball_size >= player_paddle_y &&
            ball_y <= player_paddle_y + paddle_height) {
            ball_dx = -ball_dx;
            ball_x = player_paddle_x + paddle_width; // Prevent ball from getting stuck
        }

        // Ball collision with bot paddle
        if (ball_x + ball_size >= bot_paddle_x &&
            ball_y + ball_size >= bot_paddle_y &&
            ball_y <= bot_paddle_y + paddle_height) {
            ball_dx = -ball_dx;
            ball_x = bot_paddle_x - ball_size; // Prevent ball from getting stuck
        }

        // Ball out of bounds (reset to center and update score)
        if (ball_x < 0) {
            bot_score++;
            ball_x = new_window_width / 2;
            ball_y = new_window_height / 2;
            ball_dx = 2; // Serve towards the player
        }
        if (ball_x > new_window_width) {
            player_score++;
            ball_x = new_window_width / 2;
            ball_y = new_window_height / 2;
            ball_dx = -2; // Serve towards the bot
        }

        zl_qupdate_window(window);
        zl_qdo_events(window);

        // Cap the framerate to 60fps
        double dt = zl_get_delta_time();
        if (dt < 1.0 / 60.0) {
            zl_qsleep((unsigned int)((1.0 / 60.0 - dt) * 1000));
        }
    }
}

int main() {
    zl_init(ZL_TRUE);

    char* windows_title = "Pong Game";

    window = zl_qcreate_window(windows_title, new_window_width, new_window_height);

    if (!window) {
        zl_qwriteln("Failed to create window!");
        zl_deinit();
        return 1;
    }

    zl_register_on_resize(on_window_resize);

    zl_register_on_close(on_window_close);

    zl_qset_window_title(window, windows_title);

    pong_game_loop();

    zl_qdestroy_window(window);
    zl_deinit();
    return 0;
}
