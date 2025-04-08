#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#define USE_CUSTOM_FONT
#define RUIES_IMPLEMENTATION
#include "../lib/ruies.h"

#include "../include/game_screens.h"
#include "../include/game.h"
#include "../include/snake.h"

#include "../include/fonts/author_font.h"
#include "../include/fonts/elem_font.h"
#include "../include/fonts/title_font.h"
#include "../include/fonts/font.h"

/* 
 * TODO: Make end game screen and make it possible to restart
 * TODO: Make leadership board
 */

/*
 * DONE: Make title screen
 * DONE: Make score count and time count
 */

#define STATS_SPACE 25.0f
typedef struct SeeSnakeTime {
    unsigned long secs;
    unsigned long mins;
    unsigned long hours;
} seesnake_time_t;

static void print_tail_pos(game_t game);
void update_timer(seesnake_time_t* timer);

void update_draw_frame(void);
int main(int argc, char* argv[]) {
    const char* title_text = "SeeSnake";
    InitWindow(WIN_WIDTH, WIN_HEIGHT, title_text);
    SetExitKey(KEY_NULL);

    SetTargetFPS(60);

    seesnake_time_t snake_time = {0L};
    /* bool player_board = false;
    if (!FileExists(PLAYERS_STATS_FILENAME)) {
        create_players(PLAYERS_STATS_FILENAME, 8);
    }
    player_t* players = load_players_data(PLAYERS_STATS_FILENAME);
    if (players == NULL) {
        TraceLog(LOG_ERROR, "Cannot load players data!\n");
        goto free_players;
    } */
    /* Font title_font = LoadFontEx(TITLE_FONT_PATH, TITLE_FONT_SIZE, NULL, 0);
    Font author_font = LoadFontEx(AUTHOR_FONT_PATH, AUTHOR_FONT_SIZE, NULL, 0);
    Font button_font = LoadFontEx(ELEM_FONT_PATH, ELEM_FONT_SIZE, NULL, 0);
    Font font = LoadFontEx(FONT_PATH, FONT_SIZE, NULL, 0); */

    Font author_font = LoadFont_AuthorFont();
    Font title_font = LoadFont_TitleFont();
    Font button_font = LoadFont_ElemFont();
    Font font = LoadFont_Font();

    ruies_load_custom_font(button_font, ELEM_FONT_SIZE);
    set_global_style(BUTTON_GRID, ATTR_BORDER_WIDTH, BORDER_WIDTH);
    Rectangle button_bounds = {BUTTON_SPACING, 400, 300, 75};
    const char* text[] = {"Start game", "Settings", "Quit game", NULL};
    Ruies_ButtonGrid_t btn_grid = make_button_grid(BUTTON_SPACING, WIN_HEIGHT-BUTTON_SPACING-button_bounds.height, 1, 3, RUIES_RECT(button_bounds), text, 150, 0);
    if (check_ruies_error()) {
        TraceLog(LOG_INFO, "Error");
        exit(1);
    }

    grid_alignment_t gridalign = TOPLEFT;
    Vector2 grid_pos = get_grid_pos(CELL_SIZE, GRID_ROWS, GRID_COLS, BORDER_WIDTH, CELL_SPACING, gridalign);
    Vector2 grid_dim = get_grid_dim(CELL_SIZE, GRID_ROWS, GRID_COLS, BORDER_WIDTH, CELL_SPACING);
    Vector2 score_pos = {
        .x = grid_dim.x+(2.0f*grid_pos.x),
        .y = grid_pos.y,
    };
    Vector2 paused_dims = {
        .x = WIN_WIDTH/3.0f,
        .y = WIN_HEIGHT/3.0f,
    };
    Vector2 paused_pos = {
        .x = (WIN_WIDTH-paused_dims.x)/2.0f,
        .y = (WIN_HEIGHT-paused_dims.y)/2.0f,
    };

    game_event_t event = 0x00;
    int tail_lenght = INIT_TAIL_LEN;
    game_t game = {0};
    init_game(&game, Vector2AddValue(grid_pos, (float)BORDER_WIDTH+CELL_SPACING), (Vector2){CELL_SIZE, CELL_SIZE}, tail_lenght, 10.0f);

    SetRandomSeed(time(NULL));
    set_random_snake_spawn(&game, 5);
    set_random_snake_direction(&game);
    set_snake_tail(&game, game.snake_head_direction);
    set_random_food_spawn(&game);

    const char* demo_text = "DEMO!!!";
    Vector2 demo_text_pos = {
        .x = (WIN_WIDTH-MeasureTextEx(font, demo_text, FONT_SIZE, 0).x)/2.0f,
        .y = (WIN_HEIGHT-MeasureTextEx(font, demo_text, FONT_SIZE, 0).y)/2.0f,
    };

    const char* settings_text = "Work in progress...";
    Vector2 text_pos = {
        .x = (WIN_WIDTH-MeasureTextEx(font, settings_text, FONT_SIZE, 0).x)/2.0f,
        .y = (WIN_HEIGHT-MeasureTextEx(font, settings_text, FONT_SIZE, 0).y)/2.0f,
    };

    #ifdef DEBUG
    printf("Current facing direction: %d\n", game.snake_head_direction);
    #endif
    float frame_time = 0.0f; 
    float snake_time_frame = 0.0f; 
    while (!WindowShouldClose() && game.current_action != QUIT_GAME) {
        if (game.mstate == PAUSED || game.mstate == RUNNING) {
            if (IsKeyPressed(KEY_SPACE)) {
                if (game.mstate == PAUSED) game.mstate = RUNNING;
                else game.mstate = PAUSED;
            }
        }
        if (game.mstate == RUNNING || game.mstate == SETTINGS_MENU) {
            snake_time_frame += GetFrameTime();
            if (snake_time_frame >= 1.0f) {
                update_timer(&snake_time);
                snake_time_frame = 0.0f;
            }
            get_game_event(game, &event);
            frame_time += GetFrameTime();
            if (frame_time >= (1.0f/game.snake_velocity)) {
                update_game(&game, event);
                unset_unused_tail_directions(&game);
                event = 0x00;
                frame_time = 0.0f;
            }
        }
        BeginDrawing();
        {
            ClearBackground(COLOR_BG);
            if (game.mstate == MAIN_MENU) {
                draw_title_screen(title_font, author_font, btn_grid, &game.next_action);
                DrawTextEx(font, demo_text, demo_text_pos, FONT_SIZE, 0, RED);
            }
            if (game.mstate == SETTINGS_MENU) {
                DrawTextEx(font, settings_text, text_pos, FONT_SIZE, 0, COLOR_FG);
            }
            if (game.mstate != MAIN_MENU && game.mstate != SETTINGS_MENU) {
                draw_game_title(grid_pos, grid_dim, title_text, font);
                DrawTextEx(font, TextFormat("SCORE: %ld", game.score), score_pos, FONT_SIZE, 0, COLOR_FG);
                DrawTextEx(font, TextFormat("Tail lenght: %d", game.tail_lenght), 
                           (Vector2){score_pos.x, score_pos.y+STATS_SPACE}, FONT_SIZE, 0, COLOR_FG);
                DrawTextEx(font, TextFormat("Time: %02d:%02d:%02d", snake_time.hours, snake_time.mins, snake_time.secs),
                           (Vector2){score_pos.x, score_pos.y+(2.0f*STATS_SPACE)}, FONT_SIZE, 0, COLOR_FG);
                DrawTextEx(font, TextFormat("Speed: %.2fcells/sec", game.snake_velocity), 
                           (Vector2){score_pos.x, score_pos.y+(3.0f*STATS_SPACE)}, FONT_SIZE, 0, COLOR_FG);
    /*             draw_player_board(players); */
                render_game_grid(game, grid_pos, grid_dim);
                if (game.mstate == PAUSED) {
                    draw_pause_box(paused_pos, paused_dims, font, "PAUSED", BORDER_WIDTH);
                }
            }
        }
        EndDrawing();
        update_game_according_to_action(&game);
    }
    CloseWindow();
    free(game.tail_cell_nums);
    free(game.tail_directions);
    return 0;
    /* free_players:
        free(players); */
}

static void print_tail_pos(game_t game) {
    for (int i = 0; i < game.tail_lenght; ++i) {
        printf("Tail[%d]: cell_num: %d\n", i, game.tail_cell_nums[i]);
    }
}

void update_timer(seesnake_time_t* timer) {
    if (timer->secs != 59) {
        ++timer->secs;
    } else {
        timer->secs = 0;
        if (timer->mins != 59) {
            ++timer->mins;
        } else {
            timer->mins = 0;
            if (timer->hours != 23) {
                ++timer->hours;
            }
        }
    }
}
