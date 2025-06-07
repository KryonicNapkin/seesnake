#ifndef GAME_H_
#define GAME_H_

#include "raylib.h"

#include "../lib/ruies.h"

#include "grid.h"
#include "theme.h"
#include "game_events.h"
#include "game_screens.h"
#include "snake.h"

#include "direction.h"

#define GAME_VERSION      "1.1.0-beta"

#define WIN_WIDTH         800
#define WIN_HEIGHT        600

#define ELEM_BORDER_WIDTH 2

#define GAMEWIN_DIMS      (Vector2){WIN_WIDTH, WIN_HEIGHT}

#define NULL_POS          (Vector2Zero())

typedef struct Game {
    snake_game_grid_t* grid;
    snake_t* snake;

    game_state_t state;
    game_action_t action;

    unsigned long score;

    game_theme_t theme;
    unsigned long game_time_s;

    int quit;
    void (*restart)(struct Game** self);
} game_t;

void game_init(game_t** game, snake_game_grid_t* grid, snake_t* snake, game_theme_t theme);
void game_free(game_t* game);

void game_render(game_t* game);

void draw_stats(game_t* game, game_theme_t theme);

void draw_game_title(Vector2 grid_pos, Vector2 grid_dim, const char* text, game_theme_t theme);

void update_game(game_t** game, game_event_t event);
void get_game_event(game_t* game, game_event_t* event);

void restart(game_t** game);
void update_game_according_to_action(game_t** game);

#endif /* SEESNAKE_GRID_H_ */
