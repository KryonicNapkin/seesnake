#ifndef SNAKE_H_
#define SNAKE_H_

#define SNAKE_FOOD_COLOR        (Color){0xFB, 0x49, 0x34, 0xFF} 
#define SNAKE_FOOD_COLOR_BORDER (Color){0xCC, 0x24, 0x1D, 0xFF} 

#include "game.h"
#include "direction.h"

void change_snake_direction_up(game_t* grid);
void change_snake_direction_down(game_t* grid);
void change_snake_direction_right(game_t* grid);
void change_snake_direction_left(game_t* grid);

bool is_snake_head_pos_set(game_t grid);
cell_t get_snake_head_pos(game_t grid);
direction_t get_head_direction(game_t grid);
 
bool is_snake_food_pos_set(game_t grid);
cell_t get_snake_food_pos(game_t grid);
 
void set_snake_tail(game_t* grid, direction_t direction);
void update_snake_tail_pos(game_t* grid);
void unset_unused_tail_directions(game_t* grid);
void increase_tail_size(game_t* grid);

void set_random_snake_spawn(game_t* grid, int lines_excluded_from_edges);
void set_random_food_spawn(game_t* grid);
void set_random_snake_direction(game_t* grid);
void update_snake_head_pos(game_t* grid);
#endif /* SNAKE_H_ */
