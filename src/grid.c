#include <stdlib.h>

#include "../include/grid.h"
#include "../include/game.h"

#include "raymath.h"

snake_game_grid_t* grid_make(const int size, const int cell_size, const int cell_spacing, const int grid_border_px) {
    snake_game_grid_t* grid = calloc(1, sizeof(snake_game_grid_t));

    Vector2 pos = grid_pos_get(cell_size, size, grid_border_px, cell_spacing);
    Vector2 dims = grid_dim_get(cell_size, size, grid_border_px, cell_spacing);

    Vector2 cells_pos = Vector2AddValue(pos, (float)(grid_border_px+cell_spacing));

    Vector2 init_pos = cells_pos;
    grid->cells = calloc((size*size), sizeof(cell_t));
    
    Vector2 cell_dim = {cell_size, cell_size};
    int i = 0;
    cell_pos_t place = NO_SPECIAL_PLACE;
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            grid->cells[i].drawing_pos = cells_pos;
            grid->cells[i].drawing_dims = cell_dim;
            grid->cells[i].tail_reference_dir = NO_CHANGE;
            grid->cells[i].grid_idx = i;
            grid->cells[i].x_offset = c;
            grid->cells[i].y_offset = r;

            if (c != GRID_SIZE-1) cells_pos.x += (cell_dim.x + cell_spacing);
            else cells_pos.x += cell_dim.x;

            /* Sets special attributes for corner and edge cells of the grid */
            if (r == 0) place = TOP_CELL;
            if (r == GRID_SIZE-1) place = BOTTOM_CELL;
            if (c == 0) place = LEFT_CELL;
            if (c == GRID_SIZE-1) place = RIGHT_CELL;

            if (r == 0 && c == 0) place = TOPLEFT_CELL;
            if (r == 0 && c == GRID_SIZE-1) place = TOPRIGHT_CELL;
            if (r == GRID_SIZE-1 && c == 0) place = BOTTOMLEFT_CELL;
            if (r == GRID_SIZE-1 && c == GRID_SIZE-1) place = BOTTOMRIGHT_CELL;

            if (r != 0 && r != GRID_SIZE-1 && c != 0 && c != GRID_SIZE-1) {
                place = NO_SPECIAL_PLACE;
            }

            grid->cells[i].placement = place;
            ++i;
        }
        if (r != GRID_SIZE-1) cells_pos.y += (cell_dim.y + cell_spacing);
        else cells_pos.y += cell_dim.y;

        cells_pos.x = init_pos.x;
    }
    grid->cell_spacing = cell_spacing;
    grid->cell_size = cell_size;
    grid->grid_border_px = grid_border_px;
    grid->size = size;
    grid->pos = pos;
    grid->dims = dims;
    return grid;
}

void grid_free(snake_game_grid_t* grid) {
    free(grid->cells);
    free(grid);
}

Vector2 grid_dim_get(const int cell_size, const int grid_size, const int border_width, const int cell_spacing) {
    return (Vector2){
        .x = (2.0f*border_width)+(2.0f*cell_spacing)+(grid_size*cell_size)+((grid_size-1)*cell_spacing),
        .y = (2.0f*border_width)+(2.0f*cell_spacing)+(grid_size*cell_size)+((grid_size-1)*cell_spacing),
    };
}

Vector2 grid_pos_get(const int cell_size, const int grid_size, const int border_width, const int cell_spacing) {
    Vector2 grid_pos = {0};
    Vector2 grid_dims = grid_dim_get(cell_size, grid_size, border_width, cell_spacing);

    grid_pos.x = (WIN_WIDTH-grid_dims.x)/2.0f;
    grid_pos.y = (WIN_HEIGHT-grid_dims.y)/2.0f;
    return grid_pos;
}

int cell_pos(const int x, const int y, const int grid_size) {
    return (y*grid_size)+x;
}
