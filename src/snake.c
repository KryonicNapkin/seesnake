#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "raylib.h"

#include "../include/game.h"
#include "../include/snake.h"

void change_snake_direction_up(game_t* grid) {
    if (grid->snake_head_direction != DOWN) {
        grid->snake_head_direction = UP;
        grid->cells[grid->snake_head_cell].tail_reference_dir = UP;
    }
}

void change_snake_direction_down(game_t* grid) {
    if (grid->snake_head_direction != UP) {
        grid->snake_head_direction = DOWN;
        grid->cells[grid->snake_head_cell].tail_reference_dir = DOWN;
    }
}

void change_snake_direction_right(game_t* grid) {
    if (grid->snake_head_direction != LEFT) {
        grid->snake_head_direction = RIGHT;
        grid->cells[grid->snake_head_cell].tail_reference_dir = RIGHT;
    }
}

void change_snake_direction_left(game_t* grid) {
    if (grid->snake_head_direction != RIGHT) {
        grid->snake_head_direction = LEFT;
        grid->cells[grid->snake_head_cell].tail_reference_dir = LEFT;
    }
}

bool is_snake_head_pos_set(game_t grid) {
    if (grid.snake_head_cell != UNSET_CELL) return true;
    return false;
}

direction_t get_head_direction(game_t grid) {
    return grid.snake_head_direction;
}

bool is_snake_food_pos_set(game_t grid) {
    if (grid.snake_food_cell != UNSET_CELL) return true;
    return false;
}

cell_t get_snake_food_pos(game_t grid) {
    return grid.cells[grid.snake_food_cell];
}

void set_snake_tail(game_t* grid, direction_t direction) {
    int head_cell_num = grid->snake_head_cell;
    int cell = 0;
    for (int i = 0; i < grid->tail_lenght; ++i) {
        switch (direction) {
            case LEFT:
                cell = head_cell_num+(i+1);
                break;
            case RIGHT:
                cell = head_cell_num-(i+1);
                break;
            case UP:
                cell = head_cell_num+((i+1)*GRID_COLS);
                break;
            case DOWN:
                cell = head_cell_num-((i+1)*GRID_COLS);
                break;
            case NO_CHANGE:
            default:
                break;
        }
        grid->tail_cell_nums[i] = cell;
    }
}

void update_snake_tail_pos(game_t* grid) {
    int len = grid->tail_lenght;
    int next_tail_pos = 0;
    #ifdef DEBUG
    printf("\n");
    #endif
    for (int i = 0; i < len; ++i) {
        int tail_cell = grid->tail_cell_nums[i];
        switch (grid->tail_directions[i]) {
            case LEFT:
                next_tail_pos = tail_cell-1;
                if (grid->cells[tail_cell].grid_placement & LEFT_CELL ||
                    grid->cells[tail_cell].grid_placement & TOPLEFT_CELL ||
                    grid->cells[tail_cell].grid_placement & BOTTOMLEFT_CELL) {
                    next_tail_pos = CELL_POS(GRID_COLS-1, grid->cells[tail_cell].y_offset);
                }
                #ifdef DEBUG
                printf("Current facing dir of tail_cell[%d]: %d, cell_num: %d, next_cell: %d\n", i, grid->tail_directions[i], grid->tail_cell_nums[i], next_tail_pos);
                #endif
                break;
            case RIGHT:
                next_tail_pos = tail_cell+1;
                if (grid->cells[tail_cell].grid_placement & RIGHT_CELL ||
                    grid->cells[tail_cell].grid_placement & TOPRIGHT_CELL ||
                    grid->cells[tail_cell].grid_placement & BOTTOMRIGHT_CELL) {
                    next_tail_pos = CELL_POS(0, grid->cells[tail_cell].y_offset);
                }
                #ifdef DEBUG
                printf("Current facing dir of tail_cell[%d]: %d, cell_num: %d, next_cell: %d\n", i, grid->tail_directions[i], grid->tail_cell_nums[i], next_tail_pos);
                #endif
                break;
            case UP:
                next_tail_pos = tail_cell-GRID_COLS;
                if (grid->cells[tail_cell].grid_placement & TOP_CELL ||
                    grid->cells[tail_cell].grid_placement & TOPLEFT_CELL ||
                    grid->cells[tail_cell].grid_placement & TOPRIGHT_CELL) {
                    next_tail_pos = CELL_POS(grid->cells[tail_cell].x_offset, GRID_ROWS-1);
                }
                #ifdef DEBUG
                printf("Current facing dir of tail_cell[%d]: %d, cell_num: %d, next_cell: %d\n", i, grid->tail_directions[i], grid->tail_cell_nums[i], next_tail_pos);
                #endif
                break;
            case DOWN:
                next_tail_pos = tail_cell+GRID_COLS;
                if (grid->cells[tail_cell].grid_placement & BOTTOM_CELL ||
                    grid->cells[tail_cell].grid_placement & BOTTOMLEFT_CELL ||
                    grid->cells[tail_cell].grid_placement & BOTTOMRIGHT_CELL) {
                    next_tail_pos = CELL_POS(grid->cells[tail_cell].x_offset, 0);
                }
                #ifdef DEBUG
                printf("Current facing dir of tail_cell[%d]: %d, cell_num: %d, next_cell: %d\n", i, grid->tail_directions[i], grid->tail_cell_nums[i], next_tail_pos);
                #endif
                break;
            case NO_CHANGE:
            default:
                break;
        }
        grid->tail_cell_nums[i] = next_tail_pos;
    }
    /* Sets the new facing direction of one tail tile by the tail_refrence_dir of the cell */
    for (int i = 0; i < len; ++i) {
        if (grid->cells[grid->tail_cell_nums[i]].tail_reference_dir != NO_CHANGE) {
            grid->tail_directions[i] = grid->cells[grid->tail_cell_nums[i]].tail_reference_dir;
        }
    }
}

void increase_tail_size(game_t* grid) {
    int lenght = grid->tail_lenght;
    int* new_tails = realloc(grid->tail_cell_nums, (lenght+1)*sizeof(int));
    direction_t* new_tail_dirs = realloc(grid->tail_directions, (lenght+1)*sizeof(direction_t));
    if (new_tails == NULL || new_tail_dirs == NULL) {
        TraceLog(LOG_ERROR, "Cannot increase tail size!");
    }
    grid->tail_cell_nums = new_tails;
    grid->tail_directions = new_tail_dirs;
    grid->tail_lenght = lenght+1;
    direction_t direction = grid->tail_directions[lenght-1];
    int cell_num = 0;
    /* 
     * TODO: Rework this part! My math is really bad :(
     */
    int last_tail_cell_num = grid->tail_cell_nums[lenght-1];
    switch (direction) {
        case LEFT:
            cell_num = last_tail_cell_num+1;
            if (grid->cells[last_tail_cell_num].grid_placement & RIGHT_CELL ||
                grid->cells[last_tail_cell_num].grid_placement & TOPRIGHT_CELL ||
                grid->cells[last_tail_cell_num].grid_placement & BOTTOMRIGHT_CELL) {
                cell_num = CELL_POS(0, grid->cells[last_tail_cell_num].y_offset);
            }
            break;
        case RIGHT:
            cell_num = last_tail_cell_num-1;
            if (grid->cells[last_tail_cell_num].grid_placement & LEFT_CELL ||
                grid->cells[last_tail_cell_num].grid_placement & TOPLEFT_CELL ||
                grid->cells[last_tail_cell_num].grid_placement & BOTTOMLEFT_CELL) {
                cell_num = CELL_POS(GRID_COLS-1, grid->cells[last_tail_cell_num].y_offset);
            }
            break;
        case UP:
            cell_num = last_tail_cell_num+GRID_COLS;
            if (grid->cells[last_tail_cell_num].grid_placement & BOTTOM_CELL ||
                grid->cells[last_tail_cell_num].grid_placement & BOTTOMLEFT_CELL ||
                grid->cells[last_tail_cell_num].grid_placement & BOTTOMRIGHT_CELL) {
                cell_num = CELL_POS(grid->cells[last_tail_cell_num].x_offset, 0);
            }
            break;
        case DOWN:
            cell_num = last_tail_cell_num-GRID_COLS;
            if (grid->cells[last_tail_cell_num].grid_placement & TOP_CELL ||
                grid->cells[last_tail_cell_num].grid_placement & TOPLEFT_CELL ||
                grid->cells[last_tail_cell_num].grid_placement & TOPRIGHT_CELL) {
                cell_num = CELL_POS(grid->cells[last_tail_cell_num].x_offset, GRID_ROWS-1);
            }
            break;
        case NO_CHANGE:
        default:
            break;
    }

    grid->tail_directions[lenght] = direction;
    grid->tail_cell_nums[lenght] = cell_num;
}

void unset_unused_tail_directions(game_t* grid) {
    bool used_tails;
    for (int i = 0; i < GRID_ROWS*GRID_COLS; ++i) {
        for (int j = 0; j < grid->tail_lenght; ++j) {
            if (i == grid->tail_cell_nums[j]) {
                used_tails = true;
                break;
            }
        }
        if (!used_tails && i != grid->snake_head_cell) {
            grid->cells[i].tail_reference_dir = NO_CHANGE;
        }
        used_tails = false;
    }
}

void set_random_snake_spawn(game_t* grid, int lines_excluded_from_edges) {
    int size = GRID_COLS-(2*lines_excluded_from_edges);
    int not_excluded_cell[size];

    for (int i = 0; i < size; ++i) {
        not_excluded_cell[i] = GetRandomValue(((GRID_COLS*lines_excluded_from_edges)+lines_excluded_from_edges)+(i*GRID_COLS), (GRID_COLS*(1+lines_excluded_from_edges)-(1+lines_excluded_from_edges))+(i*GRID_COLS));
    }
    int random_cell = not_excluded_cell[GetRandomValue(0, size-1)];
    grid->snake_head_cell = random_cell;
}

void set_random_food_spawn(game_t* grid) {
    int random_cell = 0;
    new_spawn:
         random_cell = GetRandomValue(0, (GRID_COLS*GRID_ROWS)-1);
    for (int i = 0; i < grid->tail_lenght; ++i) {
        if (random_cell == grid->tail_cell_nums[i]) goto new_spawn;
    }
    grid->snake_food_cell = random_cell;
}

void set_random_snake_direction(game_t* game) {
    direction_t random_direction = GetRandomValue(1, 4);
    game->snake_head_direction = random_direction;
    for (int i = 0; i < game->tail_lenght; ++i) {
        game->tail_directions[i] = random_direction;
    }
}

void update_snake_head_pos(game_t* grid) {
    int snake_head_pos = grid->snake_head_cell;
    int next_snake_pos = 0;
    cell_pos cell_grid_placement = grid->cells[snake_head_pos].grid_placement;
    switch (grid->snake_head_direction) {
        case LEFT:
            next_snake_pos = snake_head_pos-1;
            if (cell_grid_placement & LEFT_CELL ||
                cell_grid_placement & TOPLEFT_CELL ||
                cell_grid_placement & BOTTOMLEFT_CELL) {
                next_snake_pos = CELL_POS(GRID_COLS-1, grid->cells[snake_head_pos].y_offset);
            }
            break;
        case RIGHT:
            next_snake_pos = snake_head_pos+1;
            if (cell_grid_placement & RIGHT_CELL ||
                cell_grid_placement & TOPRIGHT_CELL ||
                cell_grid_placement & BOTTOMRIGHT_CELL) {
                next_snake_pos = CELL_POS(0, grid->cells[snake_head_pos].y_offset);
            }
            break;
        case UP:
            next_snake_pos = snake_head_pos-GRID_COLS;
            if (cell_grid_placement & TOP_CELL ||
                cell_grid_placement & TOPLEFT_CELL ||
                cell_grid_placement & TOPRIGHT_CELL) {
                next_snake_pos = CELL_POS(grid->cells[snake_head_pos].x_offset, GRID_ROWS-1);
            }
            break;
        case DOWN:
            next_snake_pos = snake_head_pos+GRID_COLS;
            if (cell_grid_placement & BOTTOM_CELL ||
                cell_grid_placement & BOTTOMLEFT_CELL ||
                cell_grid_placement & BOTTOMRIGHT_CELL) {
                next_snake_pos = CELL_POS(grid->cells[snake_head_pos].x_offset, 0);
            }
            break;
        case NO_CHANGE:
        default:
            break;
    }
    #ifdef DEBUG 
        printf("Head's direction: %d, prev_cell: %d, next_head_cell: %d\n", grid->snake_head_direction, snake_head_pos, next_snake_pos);
    #endif
    grid->snake_head_cell = next_snake_pos;
}
