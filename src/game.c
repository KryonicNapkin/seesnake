#include "raylib.h"
#include "raymath.h"

#include <stdio.h>
#include <stdlib.h>

#include "../include/game.h"
#include "../include/theme.h"
#include "../include/game_screens.h"
#include "../include/snake.h"

void game_init(game_t** game, snake_game_grid_t* grid, snake_t* snake, game_theme_t theme) {
    *game = calloc(1, sizeof(game_t));
    (*game)->grid = grid;
    (*game)->snake = snake;
    (*game)->theme = theme;

    (*game)->restart = restart;
    (*game)->quit = 0;
    (*game)->state = MAIN_MENU;
    (*game)->action = EMPTY;
    (*game)->game_time_s = 0;
    (*game)->score = 0;
}

void game_free(game_t* game) {
    snake_free(game->snake);
    grid_free(game->grid);
    free(game);
}

void game_render(game_t* game) {
    cell_t snake_head_cell = game->grid->cells[game->snake->head_cell_num];
    cell_t snake_food_cell = game->grid->cells[game->snake->food_cell_num];
    float circle_radius_border = (game->grid->cell_size/2.0f)-1.0f;
    float circle_radius = (game->grid->cell_size/2.0f)-2.0f;
    Vector2 center = {
        .x = snake_food_cell.drawing_pos.x+((snake_food_cell.drawing_dims.x)/2.0f),
        .y = snake_food_cell.drawing_pos.y+((snake_food_cell.drawing_dims.y)/2.0f),
    };
    /* Draw border */
    DrawRectangleV(game->grid->pos, game->grid->dims, game->theme.colors[COLOR_GRID_BORDER]);
    DrawRectangleV(Vector2AddValue(game->grid->pos, (float)GRID_BORDER_WIDTH), 
                   Vector2SubtractValue(game->grid->dims, (float)(2.0f*GRID_BORDER_WIDTH)), 
                   game->theme.colors[COLOR_BACKGROUND]);

    /* Main rendering block */
    for (int i = 0; i < game->grid->size * game->grid->size; ++i) {
        /* Draw snake's head */
        if (i == snake_head_cell.grid_idx) {
            DrawRectangleV(snake_head_cell.drawing_pos, snake_head_cell.drawing_dims, game->theme.colors[COLOR_SNAKE_HEAD]);
        } else if (i == snake_food_cell.grid_idx) {
        /* Draw snake's food */
            DrawCircleV(center, circle_radius_border, game->theme.colors[COLOR_SNAKE_FOOD_BORDER]);
            DrawCircleV(center, circle_radius, game->theme.colors[COLOR_SNAKE_FOOD_FILL]);
        } else {
        /* Draw empty cells */
            DrawRectangleV(game->grid->cells[i].drawing_pos, game->grid->cells[i].drawing_dims, game->theme.colors[COLOR_CELL_INACTIVE]);
        } 
    }
    /* Draw tail cells */
    for (int j = 0; j < game->snake->tail_lenght; ++j) {
        DrawRectangleV(game->grid->cells[game->snake->tail_cell_nums[j]].drawing_pos, 
                       game->grid->cells[game->snake->tail_cell_nums[j]].drawing_dims,
                       game->theme.colors[COLOR_CELL_ACTIVE]);
    }
}

void draw_game_title(Vector2 grid_pos, Vector2 grid_dim, const char* text, game_theme_t theme) {
    Vector2 title_pos = {0};
    title_pos.x = grid_pos.x+(grid_dim.x-MeasureTextEx(theme.default_font, text, theme.font_size, 0).x)/2.0f;
    title_pos.y = ((((WIN_HEIGHT-grid_dim.y)/2.0f)-MeasureTextEx(theme.default_font, text, theme.font_size, 0).y)/2.0f);
    DrawTextEx(theme.default_font, text, title_pos, theme.font_size, 0, theme.colors[COLOR_FOREGROUND]);
}


void update_game(game_t** game, game_event_t event) {
    if (event & EVENT_RESTART_GAME) (*game)->restart(game);
    if (event & EVENT_TO_THE_MAIN_MENU) (*game)->state = MAIN_MENU;

    if (event & EVENT_CHANGE_MOVE_UP) change_snake_direction_up((*game)->snake, &(*game)->grid);
    else if (event & EVENT_CHANGE_MOVE_DOWN) change_snake_direction_down((*game)->snake, &(*game)->grid);
    else if (event & EVENT_CHANGE_MOVE_LEFT) change_snake_direction_left((*game)->snake, &(*game)->grid);
    else if (event & EVENT_CHANGE_MOVE_RIGHT) change_snake_direction_right((*game)->snake, &(*game)->grid);

    if (event & EVENT_HEAD_TAIL_COLLISION) {
        (*game)->action = SHOW_END_SCREEN;
    }
    if (event & EVENT_FOOD_COLLISION) {
        set_random_food_spawn(&(*game)->snake, (*game)->grid->size);
        increase_tail_size(&(*game)->snake, (*game)->grid);
        (*game)->score++;
    } 
    if ((*game)->state != PAUSED) {
        update_snake_head_pos(&(*game)->snake, (*game)->grid);
        update_snake_tail_pos(&(*game)->snake, (*game)->grid);
    }
}

void get_game_event(game_t* game, game_event_t* event) {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) *event |= EVENT_CHANGE_MOVE_UP;
    else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) *event |= EVENT_CHANGE_MOVE_LEFT;
    else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) *event |= EVENT_CHANGE_MOVE_RIGHT;
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) *event |= EVENT_CHANGE_MOVE_DOWN;

    if (IsKeyPressed(KEY_R)) *event |= EVENT_RESTART_GAME; 
    else if (IsKeyPressed(KEY_SPACE)) *event |= EVENT_PAUSE_GAME;
    else if (IsKeyPressed(KEY_ESCAPE)) *event |= EVENT_TO_THE_MAIN_MENU;

    if (game->snake->head_cell_num == game->snake->food_cell_num) *event |= EVENT_FOOD_COLLISION;

    for (int i = 0; i < game->snake->tail_lenght; ++i) {
        if (game->snake->head_cell_num == game->snake->tail_cell_nums[i]) {
            *event |= EVENT_HEAD_TAIL_COLLISION;
            break;
        }
    }
    if (game->state != PAUSED) *event |= EVENT_UPDATE;
}

void restart(game_t** game) {
    (*game)->score = 0;
    (*game)->game_time_s = 0;
    snake_free((*game)->snake);
    (*game)->snake = snake_make(TAIL_LENGHT, SNAKE_VELOCITY, (*game)->grid->size);
}

void update_game_according_to_action(game_t** game) {
    if ((*game)->state == MAIN_MENU && (*game)->action == START_GAME) {
        (*game)->state = RUNNING;
    } else if ((*game)->state == MAIN_MENU && (*game)->action == QUIT_GAME) {
        (*game)->quit = 1; 
    } else if ((*game)->state == RUNNING && (*game)->action == PAUSE) {
        (*game)->state = PAUSED;
    } else if ((*game)->state == RUNNING && (*game)->action == TO_MAIN_MENU) {
        (*game)->state = MAIN_MENU;
    } else if ((*game)->state == RUNNING && (*game)->action == SHOW_END_SCREEN) {
        (*game)->state = END_SCREEN;
    } else if ((*game)->state == PAUSED && (*game)->action == TO_MAIN_MENU) {
        (*game)->state = MAIN_MENU;
    } else if ((*game)->state == PAUSED && (*game)->action == UNPAUSE) {
        (*game)->state = RUNNING;
    } else if ((*game)->state == END_SCREEN && (*game)->action == RESTART_THE_GAME) {
        (*game)->restart(game);
        (*game)->state = RUNNING;
    } else if ((*game)->state == END_SCREEN && (*game)->action == QUIT_GAME) {
        (*game)->quit = 1;
    } else (*game)->action = EMPTY;
}

void draw_stats(game_t* game, game_theme_t theme) {
    char stats[512];

    int secs = game->game_time_s % 60;
    int mins = (int)(game->game_time_s / 60);
    int hours = (int)(game->game_time_s / 3600);

    snprintf(stats, sizeof(stats), "SCORE: %ld   SPEED: %.1lfcells/sec   TAIL_LENGHT: %d    TIME: %02d:%02d:%02d", 
             game->score, game->snake->velocity, game->snake->tail_lenght, hours, mins, secs);

    Vector2 title_pos = {
        .x = game->grid->pos.x+(game->grid->dims.x-MeasureTextEx(theme.default_font, stats, theme.font_size, 0).x)/2.0f,
        .y = ((((WIN_HEIGHT-game->grid->dims.y)/2.0f)-MeasureTextEx(theme.default_font, stats, theme.font_size, 0).y)/2.0f)+game->grid->dims.y+((WIN_HEIGHT-game->grid->dims.y)/2.0f),
    };
    DrawTextEx(theme.default_font, stats, title_pos, theme.font_size, 0, theme.colors[COLOR_FOREGROUND]);
}
