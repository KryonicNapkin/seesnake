#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>

#include "../include/game.h"
#include "../include/game_screens.h"
#include "../include/snake.h"

void init_game(game_t* game, Vector2 init_pos, Vector2 cell_dims, int init_tail_lenght, float snake_velocity) {
    Vector2 pos = init_pos;
    game_style_t theme = {
        .cell_bg_color = COLOR_BG,
        .cell_fg_color = COLOR_FG,
        .border_width = BORDER_WIDTH,
        .cell_spacing = CELL_SPACING,
        .snake_head_color = BLUE,
        .snake_tail_color = COLOR_FG,
        .snake_food_border_color = SNAKE_FOOD_COLOR_BORDER,
        .snake_food_fill_color = SNAKE_FOOD_COLOR,
        .grid_border_color = COLOR_FG,
    };
    game->theme = theme;

    int i = 0;
    cell_pos grid_place = NO_SPECIAL_PLACE;
    for (int r = 0; r < GRID_ROWS; ++r) {
        for (int c = 0; c < GRID_COLS; ++c) {
            game->cells[i].drawing_pos = pos;
            game->cells[i].drawing_dims = cell_dims;
            game->cells[i].tail_reference_dir = NO_CHANGE;
            game->cells[i].grid_idx = i;
            game->cells[i].x_offset = c;
            game->cells[i].y_offset = r;
            if (c != GRID_COLS-1) {
                pos.x += (cell_dims.x + game->theme.cell_spacing);
            } else {
                pos.x += cell_dims.x;
            }
            /* Sets special attributes for corner and edge cells of the grid */
            if (r == 0) grid_place = TOP_CELL;
            if (r == GRID_ROWS-1) grid_place = BOTTOM_CELL;
            if (c == 0) grid_place = LEFT_CELL;
            if (c == GRID_COLS-1) grid_place = RIGHT_CELL;

            if (r == 0 && c == 0) grid_place = TOPLEFT_CELL;
            if (r == 0 && c == GRID_COLS-1) grid_place = TOPRIGHT_CELL;
            if (r == GRID_ROWS-1 && c == 0) grid_place = BOTTOMLEFT_CELL;
            if (r == GRID_ROWS-1 && c == GRID_COLS-1) grid_place = BOTTOMRIGHT_CELL;

            if (r != 0 && r != GRID_ROWS-1 && c != 0 && c != GRID_COLS-1) {
                grid_place = NO_SPECIAL_PLACE;
            }

            game->cells[i].grid_placement = grid_place;
            ++i;
        }
        if (r != GRID_ROWS-1) {
            pos.y += (cell_dims.y + game->theme.cell_spacing);
        } else {
            pos.y += cell_dims.y;
        }
        pos.x = init_pos.x;
    }
    int* tails = malloc(init_tail_lenght*sizeof(int));
    direction_t* directions = malloc(init_tail_lenght*sizeof(direction_t));
    game->snake_head_cell = UNSET_CELL;
    game->snake_food_cell = UNSET_CELL;
    game->score = 0;
    game->snake_velocity = snake_velocity;
    game->tail_cell_nums = tails;
    game->tail_lenght = init_tail_lenght;
    game->tail_directions = directions;
    game->restart = restart;
    game->mstate = MAIN_MENU;
    game->current_action = EMPTY;
}

void render_game_grid(game_t game, Vector2 start_pos, Vector2 start_dims) {
    cell_t snake_head_cell = game.cells[game.snake_head_cell];
    cell_t snake_food_cell = game.cells[game.snake_food_cell];
    float circle_radius_border = (game.cells->drawing_dims.x/2.0f)-1.0f;
    float circle_radius = (game.cells->drawing_dims.x/2.0f)-2.0f;
    Vector2 center = {
        .x = snake_food_cell.drawing_pos.x+((snake_food_cell.drawing_dims.x)/2.0f),
        .y = snake_food_cell.drawing_pos.y+((snake_food_cell.drawing_dims.y)/2.0f),
    };
    /* Draw border */
    DrawRectangleV(start_pos, start_dims, game.theme.grid_border_color);
    DrawRectangleV(Vector2AddValue(start_pos, (float)BORDER_WIDTH), 
                   Vector2SubtractValue(start_dims, (float)(2.0f*BORDER_WIDTH)), 
                   game.theme.cell_bg_color);

    /* Main rendering block */
    for (int i = 0; i < GRID_ROWS * GRID_COLS; ++i) {
        /* Draw snake's head */
        if (i == snake_head_cell.grid_idx) {
            DrawRectangleV(snake_head_cell.drawing_pos, snake_head_cell.drawing_dims, game.theme.snake_head_color);
        } else if (i == snake_food_cell.grid_idx) {
        /* Draw snake's food */
            DrawCircleV(center, circle_radius_border, game.theme.snake_food_border_color);
            DrawCircleV(center, circle_radius, game.theme.snake_food_fill_color);
        } else {
        /* Draw empty cells */
            DrawRectangleV(game.cells[i].drawing_pos, game.cells[i].drawing_dims, game.theme.cell_bg_color);
        } 
    }
    /* Draw tail cells */
    for (int j = 0; j < game.tail_lenght; ++j) {
        DrawRectangleV(game.cells[game.tail_cell_nums[j]].drawing_pos,
                       game.cells[game.tail_cell_nums[j]].drawing_dims,
                       game.theme.snake_tail_color);
    }
}

void draw_game_title(Vector2 grid_pos, Vector2 grid_dim, const char* text, Font font) {
    Vector2 title_pos = {0};
    title_pos.x = grid_pos.x+(grid_dim.x-MeasureTextEx(font, text, font.baseSize, 0).x)/2.0f;
    title_pos.y = ((((WIN_HEIGHT-grid_dim.y)/2.0f)-MeasureTextEx(font, text, font.baseSize, 0).y)/2.0f);
    DrawTextEx(font, text, title_pos, font.baseSize, 0, COLOR_FG);
}

void update_game(game_t* game, game_event_t event) {
    if (event & EVENT_RESTART_GAME) game->restart(game);
    if (event & EVENT_TO_THE_MAIN_MENU) game->mstate = MAIN_MENU;

    if (event & EVENT_CHANGE_MOVE_UP) change_snake_direction_up(game);
    else if (event & EVENT_CHANGE_MOVE_DOWN) change_snake_direction_down(game);
    else if (event & EVENT_CHANGE_MOVE_LEFT) change_snake_direction_left(game);
    else if (event & EVENT_CHANGE_MOVE_RIGHT) change_snake_direction_right(game);

    if (event & EVENT_HEAD_TAIL_COLLISION) {
        TraceLog(LOG_INFO, "Game over!\n");
        TraceLog(LOG_INFO, "Your score: %d\n", game->score);
        free(game->tail_cell_nums);
        free(game->tail_directions);
        exit(EXIT_SUCCESS);
    }
    if (event & EVENT_FOOD_COLLISION) {
        set_random_food_spawn(game);
        increase_tail_size(game);
        game->score += 1;
    } 
    if (game->mstate != SETTINGS_MENU) {
        update_snake_head_pos(game);
        update_snake_tail_pos(game);
    }
}

void get_game_event(game_t game, game_event_t* event) {
    if (IsKeyPressed(KEY_UP)) *event |= EVENT_CHANGE_MOVE_UP;
    else if (IsKeyPressed(KEY_LEFT)) *event |= EVENT_CHANGE_MOVE_LEFT;
    else if (IsKeyPressed(KEY_RIGHT)) *event |= EVENT_CHANGE_MOVE_RIGHT;
    else if (IsKeyPressed(KEY_DOWN)) *event |= EVENT_CHANGE_MOVE_DOWN;

    if (IsKeyPressed(KEY_R)) *event |= EVENT_RESTART_GAME; 
    else if (IsKeyPressed(KEY_SPACE)) *event |= EVENT_PAUSE_GAME;
    else if (IsKeyPressed(KEY_ESCAPE)) *event |= EVENT_TO_THE_MAIN_MENU;

    if (game.snake_head_cell == game.snake_food_cell) {
        *event |= EVENT_FOOD_COLLISION;
    }
    for (int i = 0; i < game.tail_lenght; ++i) {
        if (game.snake_head_cell == game.tail_cell_nums[i]) {
            *event |= EVENT_HEAD_TAIL_COLLISION;
            break;
        }
    }
    if (game.mstate != PAUSED) *event |= EVENT_UPDATE;
}

void restart(game_t* game) {
    game->score = 0;
    game->tail_lenght = INIT_TAIL_LEN;
    game->snake_head_direction = NO_CHANGE;
    game->snake_head_cell = UNSET_CELL;
    game->snake_food_cell = UNSET_CELL;
    set_random_snake_spawn(game, 5);
    set_random_snake_direction(game);
    set_snake_tail(game, game->snake_head_direction);
    set_random_food_spawn(game);
}

void update_game_according_to_action(game_t* game) {
    if (game->current_action != game->next_action) {
        game->current_action = game->next_action;
        if (game->next_action == START_GAME) {
            game->mstate = RUNNING;
        } else if (game->next_action == PAUSE) {
            game->mstate = PAUSED;
        } else if (game->next_action == UNPAUSE) {
            game->mstate = RUNNING;
        } else if (game->next_action == GO_TO_SETTINGS) {
            game->mstate = SETTINGS_MENU;
        }
        game->next_action = EMPTY;
    }
}

Vector2 get_grid_pos(int cell_size, int rows, int cols, int border_width, int cell_spacing, grid_alignment_t alignment) {
    Vector2 grid_pos = {0};
    Vector2 grid_dims = get_grid_dim(cell_size, rows, cols, border_width, cell_spacing);
    switch (alignment) {
        case TOPLEFT:
            grid_pos.x = OFFSET_FROM_WIN;
            grid_pos.y = OFFSET_FROM_WIN;
            break;
        case TOPCENTER:
            grid_pos.x = (WIN_WIDTH-grid_dims.x)/2.0f;
            grid_pos.y = OFFSET_FROM_WIN;
            break;
        case TOPRIGHT:
            grid_pos.x = WIN_WIDTH-(grid_dims.x+OFFSET_FROM_WIN);
            grid_pos.y = OFFSET_FROM_WIN;
            break;
        case MIDDLELEFT:
            grid_pos.x = OFFSET_FROM_WIN;
            grid_pos.y = (WIN_HEIGHT-grid_dims.y)/2.0f;
            break;
        case MIDDLECENTER:
            grid_pos.x = (WIN_WIDTH-grid_dims.x)/2.0f;
            grid_pos.y = (WIN_HEIGHT-grid_dims.y)/2.0f;
            break;
        case MIDDLERIGHT:
            grid_pos.x = WIN_WIDTH-(grid_dims.x+OFFSET_FROM_WIN);
            grid_pos.y = (WIN_HEIGHT-grid_dims.y)/2.0f;
            break;
        case BOTTOMLEFT:
            grid_pos.x = OFFSET_FROM_WIN;
            grid_pos.y = WIN_HEIGHT-(grid_dims.y+OFFSET_FROM_WIN);
            break;
        case BOTTOMCENTER:
            grid_pos.x = (WIN_WIDTH-grid_dims.x)/2.0f;
            grid_pos.y = WIN_HEIGHT-(grid_dims.y+OFFSET_FROM_WIN);
            break;
        case BOTTOMRIGHT:
            grid_pos.x = WIN_WIDTH-(grid_dims.x+OFFSET_FROM_WIN);
            grid_pos.y = WIN_HEIGHT-(grid_dims.y+OFFSET_FROM_WIN);
            break;
        default:
            break;
    }
    return grid_pos;
}

Vector2 get_grid_dim(int cell_size, int rows, int cols, int border_width, int cell_spacing) {
    Vector2 grid_dim = {
        .x = (2.0f*border_width)+(2.0f*cell_spacing)+(cols*cell_size)+((cols-1)*cell_spacing),
        .y = (2.0f*border_width)+(2.0f*cell_spacing)+(rows*cell_size)+((rows-1)*cell_spacing),
    };
    return grid_dim;
}
