#ifndef THEME_H_
#define THEME_H_

#include "raylib.h"

/* Border width of the game grid */
#define GRID_BORDER_WIDTH            2

#define FOREGROUND_COLOR        (Color){0x28, 0x2C, 0x34, 0xFF} /* #282C34 */
#define BACKGROUND_COLOR        (Color){0x98, 0xC3, 0x79, 0xFF} /* #98C379 */ 
#define CELL_ACTIVE_COLOR       (Color){0x28, 0x2C, 0x34, 0xFF} /* #282C34 */
#define CELL_INACTIVE_COLOR     (Color){0x98, 0xC3, 0x79, 0xFF} /* #98C379 */ 
#define SNAKE_HEAD_COLOR        (Color){0x61, 0xAF, 0xEF, 0xFF} /* #61AFEF */
#define SNAKE_TAIL_COLOR        (Color){0x28, 0x2C, 0x34, 0xFF} /* #282C34 */ 
#define SNAKE_FOOD_BORDER_COLOR (Color){0x28, 0x2C, 0x34, 0xFF} /* #282C34 */
#define SNAKE_FOOD_FILL_COLOR   (Color){0xE0, 0x6C, 0x75, 0xFF} /* #E06C75 */ 
#define GRID_BORDER_COLOR       (Color){0x28, 0x2C, 0x34, 0xFF} /* #282C34 */

#define NUM_OF_COLORS           9

typedef enum ColorIndex {
    COLOR_FOREGROUND = 0,     
    COLOR_BACKGROUND,        
    COLOR_CELL_ACTIVE,       
    COLOR_CELL_INACTIVE,     
    COLOR_SNAKE_HEAD,        
    COLOR_SNAKE_TAIL,        
    COLOR_SNAKE_FOOD_BORDER, 
    COLOR_SNAKE_FOOD_FILL,   
    COLOR_GRID_BORDER,       
} color_index_t;

typedef struct GameTheme {
    Color colors[NUM_OF_COLORS];
    Font default_font;
    float font_size;
} game_theme_t;

game_theme_t get_default_theme(void);
void change_theme_color(game_theme_t* theme, color_index_t index, Color new_color);

#endif /* THEME_H_ */
