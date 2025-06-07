#ifndef SNAKE_H_
#define SNAKE_H_

#define SNAKE_FOOD_COLOR        (Color){0xFB, 0x49, 0x34, 0xFF} 
#define SNAKE_FOOD_COLOR_BORDER (Color){0xCC, 0x24, 0x1D, 0xFF} 

#include "direction.h"
#include "grid.h"

#define TAIL_LENGHT    4
/* Cells per seconds */
#define SNAKE_VELOCITY 12.5f

typedef struct Snake {
    int head_cell_num;
    direction_t head_direction;
    int food_cell_num;

    int* tail_cell_nums;
    direction_t* tail_directions;
    int tail_lenght;

    float velocity;
} snake_t;

snake_t* snake_make(const int initial_tail_lenght, const float velocity, const int grid_size);
void snake_free(snake_t* snake);

void change_snake_direction_up(snake_t* snake, snake_game_grid_t** grid);
void change_snake_direction_down(snake_t* snake, snake_game_grid_t** grid);
void change_snake_direction_right(snake_t* snake, snake_game_grid_t** grid);
void change_snake_direction_left(snake_t* snake, snake_game_grid_t** grid);

bool is_snake_head_pos_set(snake_t* snake);
cell_t get_snake_head_pos(snake_t* snake, snake_game_grid_t* grid);
direction_t get_head_direction(snake_t* snake);
 
bool is_snake_food_pos_set(snake_t* snake);
cell_t get_snake_food_pos(snake_t* snake, snake_game_grid_t* grid);
 
void set_snake_tail(snake_t** snake, const int grid_size, direction_t direction);
void update_snake_tail_pos(snake_t** snake, snake_game_grid_t* grid);
void unset_unused_tail_directions(snake_t** snake, snake_game_grid_t** grid);
void increase_tail_size(snake_t** snake, snake_game_grid_t* grid);

void set_random_snake_spawn(snake_t** snake, const int grid_size, const int lines_excluded_from_edges);
void set_random_food_spawn(snake_t** snake, const int grid_size);
void set_random_snake_direction(snake_t** snake);
void update_snake_head_pos(snake_t** snake, snake_game_grid_t* grid);
#endif /* SNAKE_H_ */
