#ifndef GRID_H_
#define GRID_H_

#include "raylib.h"

#include "theme.h"
#include "direction.h"

#define UNSET_CELL       -1

#define GRID_SIZE        27
#define CELL_SIZE        15.0f              /* In px */
#define CELL_SPACING_PX  3

#define NO_SPECIAL_PLACE 0U        
#define LEFT_CELL        (1 << 0U) /* 0x00000001 */
#define RIGHT_CELL       (1 << 1U) /* 0x00000010 */
#define TOP_CELL         (1 << 2U) /* 0x00000100 */
#define BOTTOM_CELL      (1 << 3U) /* 0x00001000 */
#define TOPLEFT_CELL     (1 << 4U) /* 0x00010000 */
#define TOPRIGHT_CELL    (1 << 5U) /* 0x00100000 */
#define BOTTOMLEFT_CELL  (1 << 6U) /* 0x01000000 */
#define BOTTOMRIGHT_CELL (1 << 7U) /* 0x10000000 */

typedef unsigned int cell_pos_t;

typedef struct Cell {
    int grid_idx;
    Vector2 drawing_pos;
    Vector2 drawing_dims;
    direction_t tail_reference_dir;
    cell_pos_t placement;

    int x_offset; 
    int y_offset;
} cell_t;

typedef struct SnakeGameGrid {
    /* The Snake's playing grid is always a square */
    /* size as in number of cells in a row */
    int size;
    int cell_size;
    int cell_spacing;
    int grid_border_px;
    
    Vector2 pos;
    Vector2 dims;
    cell_t* cells;
} snake_game_grid_t;

snake_game_grid_t* grid_make(const int size, const int cell_size, const int cell_spacing, const int grid_border_px);
void grid_free(snake_game_grid_t* grid);

void grid_recalculate(snake_game_grid_t** grid, const int size, const int cell_size, const int cell_spacing, const int grid_border_px);

void free_cells(snake_game_grid_t** grid);

Vector2 grid_pos_get(const int cell_size, const int grid_size, const int border_width, const int cell_spacing);
Vector2 grid_dim_get(const int cell_size, const int grid_size, const int border_width, const int cell_spacing);

/* Get cell index in the  grid from the x offset and y offset */
int cell_pos(const int x, const int y, const int grid_size);

#endif /* GRID_H_ */
