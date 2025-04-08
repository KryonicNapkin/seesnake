#ifndef GAME_H_
#define GAME_H_

#include "raylib.h"

#include "../lib/ruies.h"

#include "game_events.h"
#include "game_screens.h"

#include "direction.h"

#define WIN_WIDTH        800
#define WIN_HEIGHT       600
#define GAMEWIN_DIMS     (Vector2){WIN_WIDTH, WIN_HEIGHT}
#define GRID_ROWS        26
#define GRID_COLS        20
#define CELL_SIZE        15.0f              /* In px */
#define BORDER_WIDTH     3
#define CELL_SPACING     3
#define INIT_TAIL_LEN    4

#define OFFSET_FROM_WIN  20.0f

#define NULL_POS         (Vector2Zero())

#define TITLE_FONT_PATH  "assets/8bitlim.ttf"
#define TITLE_FONT_SIZE  120
#define FONT_PATH        "assets/PixeloidSansBold-PKnYd.ttf"
#define FONT_SIZE        25
#define AUTHOR_FONT_PATH "assets/8bitlim.ttf"
#define AUTHOR_FONT_SIZE 30
#define ELEM_FONT_PATH   "assets/8bitlim.ttf"
#define ELEM_FONT_SIZE   25

#define UNSET_CELL       -1
/* Get cell index in the grid from the x offset and y offset */
#define CELL_POS(x,y)    (((y)*GRID_COLS)+(x))

#define COLOR_BG         (Color){0x98, 0xC3, 0x79, 0xFF} /* #98C379 */ 
#define COLOR_FG         (Color){0x28, 0x2C, 0x34, 0xFF} /* #282C34 */

#define NO_SPECIAL_PLACE 0U        
#define LEFT_CELL        (1 << 0U) /* 0x00000001 */
#define RIGHT_CELL       (1 << 1U) /* 0x00000010 */
#define TOP_CELL         (1 << 2U) /* 0x00000100 */
#define BOTTOM_CELL      (1 << 3U) /* 0x00001000 */
#define TOPLEFT_CELL     (1 << 4U) /* 0x00010000 */
#define TOPRIGHT_CELL    (1 << 5U) /* 0x00100000 */
#define BOTTOMLEFT_CELL  (1 << 6U) /* 0x01000000 */
#define BOTTOMRIGHT_CELL (1 << 7U) /* 0x10000000 */

typedef unsigned int cell_pos;

typedef enum GridAlignment {
    TOPLEFT = 0,
    TOPCENTER,
    TOPRIGHT,
    MIDDLELEFT,
    MIDDLECENTER,
    MIDDLERIGHT,
    BOTTOMLEFT,
    BOTTOMCENTER,
    BOTTOMRIGHT
} grid_alignment_t;

typedef struct Cell {
    int grid_idx;
    Vector2 drawing_pos;
    Vector2 drawing_dims;
    direction_t tail_reference_dir;
    cell_pos grid_placement;

    int x_offset; 
    int y_offset;
} cell_t;

typedef struct GameStyle {
    Color cell_bg_color;
    Color cell_fg_color;
    int border_width;
    int cell_spacing;
    Color snake_head_color;
    Color snake_tail_color;
    Color snake_food_border_color;
    Color snake_food_fill_color;
    Color grid_border_color;
} game_style_t;

typedef struct Game {
    cell_t cells[GRID_ROWS * GRID_COLS];

    int snake_head_cell;
    direction_t snake_head_direction;
    int snake_food_cell;

    int* tail_cell_nums;
    direction_t* tail_directions;
    int tail_lenght;

    float snake_velocity;

    menu_state_t mstate;
    menu_action_t next_action;
    menu_action_t current_action;

    unsigned long long score;

    game_style_t theme;

    void (*restart)(struct Game* self);
} game_t;

void init_game(game_t* game, Vector2 init_pos, Vector2 cell_dims, 
               int init_tail_lenght, float snake_velocity);
void render_game_grid(game_t game, Vector2 start_pos, Vector2 start_dims);
void draw_game_title(Vector2 grid_pos, Vector2 grid_dim, const char* text, Font font);
void update_game(game_t* game, game_event_t event);
void get_game_event(game_t game, game_event_t* event);
void restart(game_t* game);
void update_game_according_to_action(game_t* game);

Vector2 get_grid_pos(int cell_size, int rows, int cols, int border_width, 
                     int cell_spacing, grid_alignment_t alignment);
Vector2 get_grid_dim(int cell_size, int rows, int cols, int border_width, 
                     int cell_spacing);
#endif /* SEESNAKE_GRID_H_ */
