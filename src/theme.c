#include "../include/theme.h"

#include "../include/font.h"
#include "../include/font_config.h"

game_theme_t get_default_theme(void) {
    game_theme_t theme = {0};

    theme.colors[COLOR_FOREGROUND] = FOREGROUND_COLOR;
    theme.colors[COLOR_BACKGROUND] = BACKGROUND_COLOR;
    theme.colors[COLOR_CELL_ACTIVE] = CELL_ACTIVE_COLOR;
    theme.colors[COLOR_CELL_INACTIVE] = CELL_INACTIVE_COLOR;
    theme.colors[COLOR_GRID_BORDER] = GRID_BORDER_COLOR;
    theme.colors[COLOR_SNAKE_FOOD_BORDER] = SNAKE_FOOD_BORDER_COLOR;
    theme.colors[COLOR_SNAKE_FOOD_FILL] = SNAKE_FOOD_FILL_COLOR;
    theme.colors[COLOR_SNAKE_HEAD] = SNAKE_HEAD_COLOR;
    theme.colors[COLOR_SNAKE_TAIL] = SNAKE_TAIL_COLOR;

    theme.default_font = LoadSeesnakeFont();
    theme.font_size = FONT_SIZE;

    return theme;
}

void change_theme_color(game_theme_t* theme, color_index_t index, Color new_color) {
    theme->colors[index] = new_color;
}
