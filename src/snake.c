#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "raylib.h"

#include "../include/snake.h"
#include "../include/grid.h"

snake_t* snake_make(const int initial_tail_lenght, const float velocity, const int grid_size) {
    snake_t* snake = calloc(1, sizeof(snake_t));
    TraceLog(LOG_INFO, "Grid_size1: %d\n", grid_size);

    snake->head_cell_num = UNSET_CELL;
    snake->food_cell_num = UNSET_CELL;
    snake->velocity = velocity;
    snake->tail_cell_nums = calloc(initial_tail_lenght, sizeof(int));
    snake->tail_lenght = initial_tail_lenght;
    snake->tail_directions = calloc(initial_tail_lenght, sizeof(direction_t));

    set_random_snake_spawn(&snake, grid_size, 5);
    set_random_snake_direction(&snake);
    set_snake_tail(&snake, grid_size, snake->head_direction);
    set_random_food_spawn(&snake, grid_size);

    return snake;
}

void snake_free(snake_t* snake) {
    free(snake->tail_cell_nums);
    free(snake->tail_directions);
    free(snake);
}

void change_snake_direction_up(snake_t* snake, snake_game_grid_t** grid) {
    if (snake->head_direction != DIR_DOWN) {
        snake->head_direction = DIR_UP;
        (*grid)->cells[snake->head_cell_num].tail_reference_dir = DIR_UP;
    }
}

void change_snake_direction_down(snake_t* snake, snake_game_grid_t** grid) {
    if (snake->head_direction != DIR_UP) {
        snake->head_direction = DIR_DOWN;
        (*grid)->cells[snake->head_cell_num].tail_reference_dir = DIR_DOWN;
    }
}

void change_snake_direction_right(snake_t* snake, snake_game_grid_t** grid) {
    if (snake->head_direction != DIR_LEFT) {
        snake->head_direction = DIR_RIGHT;
        (*grid)->cells[snake->head_cell_num].tail_reference_dir = DIR_RIGHT;
    }
}

void change_snake_direction_left(snake_t* snake, snake_game_grid_t** grid) {
    if (snake->head_direction != DIR_RIGHT) {
        snake->head_direction = DIR_LEFT;
        (*grid)->cells[snake->head_cell_num].tail_reference_dir = DIR_LEFT;
    }
}

bool is_snake_head_pos_set(snake_t* snake) {
    if (snake->head_cell_num != UNSET_CELL) return true;
    return false;
}

inline direction_t get_head_direction(snake_t* snake) {
    return snake->head_direction;
}

bool is_snake_food_pos_set(snake_t* snake) {
    if (snake->food_cell_num != UNSET_CELL) return true;
    return false;
}

inline cell_t get_snake_food_pos(snake_t* snake, snake_game_grid_t* grid) {
    return grid->cells[snake->food_cell_num];
}

void set_snake_tail(snake_t** snake, const int grid_size, direction_t direction) {
    int head_cell_num = (*snake)->head_cell_num;
    int cell = 0;
    for (int i = 0; i < (*snake)->tail_lenght; ++i) {
        switch (direction) {
            case DIR_LEFT:
                cell = head_cell_num+(i+1);
                break;
            case DIR_RIGHT:
                cell = head_cell_num-(i+1);
                break;
            case DIR_UP:
                cell = head_cell_num+((i+1)*grid_size);
                break;
            case DIR_DOWN:
                cell = head_cell_num-((i+1)*grid_size);
                break;
            case NO_CHANGE:
            default:
                break;
        }
        (*snake)->tail_cell_nums[i] = cell;
    }
}

void update_snake_tail_pos(snake_t** snake, snake_game_grid_t* grid) {
    int len = (*snake)->tail_lenght;
    int next_tail_pos = 0;
    for (int i = 0; i < len; ++i) {
        int tail_cell = (*snake)->tail_cell_nums[i];
        switch ((*snake)->tail_directions[i]) {
            case DIR_LEFT:
                next_tail_pos = tail_cell-1;
                if (grid->cells[tail_cell].placement & LEFT_CELL ||
                    grid->cells[tail_cell].placement & TOPLEFT_CELL ||
                    grid->cells[tail_cell].placement & BOTTOMLEFT_CELL) {
                    next_tail_pos = cell_pos(grid->size-1, grid->cells[tail_cell].y_offset, grid->size);
                }
                #ifdef DEBUG
                TraceLog(LOG_INFO, "Current facing dir of tail_cell[%d]: %d, cell_num: %d, next_cell: %d", i, (*snake)->tail_directions[i], (*snake)->tail_cell_nums[i], next_tail_pos);
                #endif
                break;
            case DIR_RIGHT:
                next_tail_pos = tail_cell+1;
                if (grid->cells[tail_cell].placement & RIGHT_CELL ||
                    grid->cells[tail_cell].placement & TOPRIGHT_CELL ||
                    grid->cells[tail_cell].placement & BOTTOMRIGHT_CELL) {
                    next_tail_pos = cell_pos(0, grid->cells[tail_cell].y_offset, grid->size);
                }
                #ifdef DEBUG
                TraceLog(LOG_INFO, "Current facing dir of tail_cell[%d]: %d, cell_num: %d, next_cell: %d", i, (*snake)->tail_directions[i], (*snake)->tail_cell_nums[i], next_tail_pos);
                #endif
                break;
            case DIR_UP:
                next_tail_pos = tail_cell-grid->size;
                if (grid->cells[tail_cell].placement & TOP_CELL ||
                    grid->cells[tail_cell].placement & TOPLEFT_CELL ||
                    grid->cells[tail_cell].placement & TOPRIGHT_CELL) {
                    next_tail_pos = cell_pos(grid->cells[tail_cell].x_offset, grid->size-1, grid->size);
                }
                #ifdef DEBUG
                TraceLog(LOG_INFO, "Current facing dir of tail_cell[%d]: %d, cell_num: %d, next_cell: %d", i, (*snake)->tail_directions[i], (*snake)->tail_cell_nums[i], next_tail_pos);
                #endif
                break;
            case DIR_DOWN:
                next_tail_pos = tail_cell+grid->size;
                if (grid->cells[tail_cell].placement & BOTTOM_CELL ||
                    grid->cells[tail_cell].placement & BOTTOMLEFT_CELL ||
                    grid->cells[tail_cell].placement & BOTTOMRIGHT_CELL) {
                    next_tail_pos = cell_pos(grid->cells[tail_cell].x_offset, 0, grid->size);
                }
                #ifdef DEBUG
                TraceLog(LOG_INFO, "Current facing dir of tail_cell[%d]: %d, cell_num: %d, next_cell: %d", i, (*snake)->tail_directions[i], (*snake)->tail_cell_nums[i], next_tail_pos);
                #endif
                break;
            case NO_CHANGE:
            default:
                break;
        }
        (*snake)->tail_cell_nums[i] = next_tail_pos;
    }
    /* Sets the new facing direction of one tail tile by the tail_refrence_dir of the cell */
    for (int i = 0; i < len; ++i) {
        if (grid->cells[(*snake)->tail_cell_nums[i]].tail_reference_dir != NO_CHANGE) {
            (*snake)->tail_directions[i] = grid->cells[(*snake)->tail_cell_nums[i]].tail_reference_dir;
        }
    }
}

void increase_tail_size(snake_t** snake, snake_game_grid_t* grid) {
    int lenght = (*snake)->tail_lenght;
    (*snake)->tail_cell_nums = realloc((*snake)->tail_cell_nums, (lenght+1) * sizeof(int));
    (*snake)->tail_directions = realloc((*snake)->tail_directions, (lenght+1) * sizeof(direction_t));
    if ((*snake)->tail_cell_nums == NULL || (*snake)->tail_directions == NULL) {
        TraceLog(LOG_ERROR, "Cannot increase tail size!");
    }
    (*snake)->tail_lenght++;
    direction_t direction = (*snake)->tail_directions[lenght-1];
    int cell_num = 0;
    /* 
     * TODO: Rework this part! My math is really bad :(
     */
    int last_tail_cell_num = (*snake)->tail_cell_nums[lenght-1];
    switch (direction) {
        case DIR_LEFT:
            cell_num = last_tail_cell_num+1;
            if (grid->cells[last_tail_cell_num].placement & RIGHT_CELL ||
                grid->cells[last_tail_cell_num].placement & TOPRIGHT_CELL ||
                grid->cells[last_tail_cell_num].placement & BOTTOMRIGHT_CELL) {
                cell_num = cell_pos(0, grid->cells[last_tail_cell_num].y_offset, grid->size);
            }
            break;
        case DIR_RIGHT:
            cell_num = last_tail_cell_num-1;
            if (grid->cells[last_tail_cell_num].placement & LEFT_CELL ||
                grid->cells[last_tail_cell_num].placement & TOPLEFT_CELL ||
                grid->cells[last_tail_cell_num].placement & BOTTOMLEFT_CELL) {
                cell_num = cell_pos(grid->size-1, grid->cells[last_tail_cell_num].y_offset, grid->size);
            }
            break;
        case DIR_UP:
            cell_num = last_tail_cell_num+grid->size;
            if (grid->cells[last_tail_cell_num].placement & BOTTOM_CELL ||
                grid->cells[last_tail_cell_num].placement & BOTTOMLEFT_CELL ||
                grid->cells[last_tail_cell_num].placement & BOTTOMRIGHT_CELL) {
                cell_num = cell_pos(grid->cells[last_tail_cell_num].x_offset, 0, grid->size);
            }
            break;
        case DIR_DOWN:
            cell_num = last_tail_cell_num-grid->size;
            if (grid->cells[last_tail_cell_num].placement & TOP_CELL ||
                grid->cells[last_tail_cell_num].placement & TOPLEFT_CELL ||
                grid->cells[last_tail_cell_num].placement & TOPRIGHT_CELL) {
                cell_num = cell_pos(grid->cells[last_tail_cell_num].x_offset, grid->size-1, grid->size);
            }
            break;
        case NO_CHANGE:
        default:
            break;
    }

    (*snake)->tail_directions[lenght] = direction;
    (*snake)->tail_cell_nums[lenght] = cell_num;
}

void unset_unused_tail_directions(snake_t** snake, snake_game_grid_t** grid) {
    bool used_tails;
    for (int i = 0; i < (*grid)->size * (*grid)->size; ++i) {
        for (int j = 0; j < (*snake)->tail_lenght; ++j) {
            if (i == (*snake)->tail_cell_nums[j]) {
                used_tails = true;
                break;
            }
        }
        if (!used_tails && i != (*snake)->head_cell_num) {
            (*grid)->cells[i].tail_reference_dir = NO_CHANGE;
        }
        used_tails = false;
    }
}

void set_random_snake_spawn(snake_t** snake, const int grid_size, const int lines_excluded_from_edges) {
    int size = grid_size-(2*lines_excluded_from_edges);
    int not_excluded_cell[size];
    TraceLog(LOG_INFO, "grid_size: %d\n", grid_size);

    for (int i = 0; i < size; ++i) {
        not_excluded_cell[i] = GetRandomValue(((grid_size*lines_excluded_from_edges)+lines_excluded_from_edges)+(i*grid_size), 
                                              ((grid_size*(1+lines_excluded_from_edges))-(1+lines_excluded_from_edges))+(i*grid_size));
    }
    int random_cell = not_excluded_cell[GetRandomValue(0, size-1)];
    printf("Random_cell: %d\n", random_cell);
    (*snake)->head_cell_num = random_cell;
}

void set_random_food_spawn(snake_t** snake, const int grid_size) {
    int random_cell = 0;
    new_spawn:
        random_cell = GetRandomValue(0, (grid_size*grid_size)-1);
        for (int i = 0; i < (*snake)->tail_lenght; ++i) {
            if (random_cell == (*snake)->tail_cell_nums[i]) goto new_spawn;
        }

    (*snake)->food_cell_num = random_cell;
}

void set_random_snake_direction(snake_t** snake) {
    direction_t random_direction = GetRandomValue(1, 4);
    (*snake)->head_direction = random_direction;
    for (int i = 0; i < (*snake)->tail_lenght; ++i) {
        (*snake)->tail_directions[i] = random_direction;
    }
}

void update_snake_head_pos(snake_t** snake, snake_game_grid_t* grid) {
    int snake_head_pos = (*snake)->head_cell_num;
    int next_snake_pos = 0;
    cell_pos_t cell_placement = grid->cells[snake_head_pos].placement;
    switch ((*snake)->head_direction) {
        case DIR_LEFT:
            next_snake_pos = snake_head_pos-1;
            if (cell_placement & LEFT_CELL ||
                cell_placement & TOPLEFT_CELL ||
                cell_placement & BOTTOMLEFT_CELL) {
                next_snake_pos = cell_pos(grid->size-1, grid->cells[snake_head_pos].y_offset, grid->size);
            }
            break;
        case DIR_RIGHT:
            next_snake_pos = snake_head_pos+1;
            if (cell_placement & RIGHT_CELL ||
                cell_placement & TOPRIGHT_CELL ||
                cell_placement & BOTTOMRIGHT_CELL) {
                next_snake_pos = cell_pos(0, grid->cells[snake_head_pos].y_offset, grid->size);
            }
            break;
        case DIR_UP:
            next_snake_pos = snake_head_pos-grid->size;
            if (cell_placement & TOP_CELL ||
                cell_placement & TOPLEFT_CELL ||
                cell_placement & TOPRIGHT_CELL) {
                next_snake_pos = cell_pos(grid->cells[snake_head_pos].x_offset, grid->size-1, grid->size);
            }
            break;
        case DIR_DOWN:
            next_snake_pos = snake_head_pos+grid->size;
            if (cell_placement & BOTTOM_CELL ||
                cell_placement & BOTTOMLEFT_CELL ||
                cell_placement & BOTTOMRIGHT_CELL) {
                next_snake_pos = cell_pos(grid->cells[snake_head_pos].x_offset, 0, grid->size);
            }
            break;
        case NO_CHANGE:
        default:
            break;
    }
    #ifdef DEBUG 
        TraceLog(LOG_INFO, "Head's direction: %d, prev_cell: %d, next_head_cell: %d", (*snake)->head_direction, snake_head_pos, next_snake_pos);
    #endif
    (*snake)->head_cell_num = next_snake_pos;
}
