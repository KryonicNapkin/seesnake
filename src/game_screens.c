#include "raylib.h"
#include "raymath.h"

#include <stddef.h>

#include "../include/game.h"
#include "../include/game_screens.h"

void draw_pause_box(Vector2 pos, Vector2 dims, Font font, const char* text, int border_width) {
    float padding = 20.0f;
    Vector2 pos2 = Vector2AddValue(pos, padding);
    Vector2 dims2 = Vector2SubtractValue(dims, 2.0f*padding);
    Vector2 pos3 = Vector2AddValue(pos2, (float)border_width);
    Vector2 dims3 = Vector2SubtractValue(dims2, 2.0f*border_width);
    Vector2 pos4 = Vector2AddValue(pos3, (float)2.0f*border_width);
    Vector2 dims4 = Vector2SubtractValue(dims3, 4.0f*border_width);
    Vector2 pos5 = Vector2AddValue(pos4, (float)border_width);
    Vector2 dims5 = Vector2SubtractValue(dims4, 2.0f*border_width);
    Vector2 text_pos = {
        .x = pos5.x+((dims5.x-MeasureTextEx(font, text, font.baseSize, 0).x)/2.0f),
        .y = pos5.y+((dims5.y-MeasureTextEx(font, text, font.baseSize, 0).y)/2.0f),
    };
    DrawRectangleV(pos, dims, COLOR_BG);
    DrawRectangleV(pos2, dims2, COLOR_FG);
    DrawRectangleV(pos3, dims3, COLOR_BG);
    DrawRectangleV(pos4, dims4, COLOR_FG);
    DrawRectangleV(pos5, dims5, COLOR_BG);
    DrawTextEx(font, text, text_pos, font.baseSize, 0, COLOR_FG);
}
/* 
 * TODO: Make end screen when you collide with your tail
 */

int draw_end_game_screen(Font font) {
    int ret = 0;
    const char* text = "COLLISION!";
    const char* text2 = "";
    Vector2 pos = Vector2Zero();
    DrawRectangleV(pos, (Vector2){WIN_WIDTH, WIN_HEIGHT}, COLOR_BG);
    Vector2 text_pos = {
        .x = (WIN_WIDTH-MeasureTextEx(font, text, font.baseSize, 0).x)/2.0f,
        .y = (WIN_HEIGHT-MeasureTextEx(font, text, font.baseSize, 0).y)/2.0f,
    };
    return ret;
}

void draw_title_screen(Font title_font, Font author_font, Ruies_ButtonGrid_t buttons, 
                       menu_action_t* action) {
    const char* title_text = "SeeSnake";
    const char* author_text = "By KryonicNapkin";

    Vector2 title_text_pos = {
        .x = (WIN_WIDTH-MeasureTextEx(title_font, title_text, TITLE_FONT_SIZE, 0).x)/2.0f,
        .y = (WIN_HEIGHT-MeasureTextEx(title_font, title_text, TITLE_FONT_SIZE, 0).y)/6.0f,
    };
    Vector2 author_text_pos = {
        .x = (WIN_WIDTH-MeasureTextEx(author_font, author_text, AUTHOR_FONT_SIZE, 0).x)/2.0f,
        .y = (WIN_HEIGHT-MeasureTextEx(author_font, author_text, AUTHOR_FONT_SIZE, 0).y)/2.5f,
    };

    DrawRectangleV(NULL_POS, GAMEWIN_DIMS, COLOR_BG);
    DrawTextEx(title_font, title_text, title_text_pos, TITLE_FONT_SIZE, 0, COLOR_FG);
    DrawTextEx(author_font, author_text, author_text_pos, AUTHOR_FONT_SIZE, 0, COLOR_FG);

    stretch_button_grid_horiz(&buttons, BUTTON_SPACING, WIN_WIDTH-BUTTON_SPACING);
    vrender_button(3, &buttons.buttons[0], &buttons.buttons[1], &buttons.buttons[2]);

    if (get_button_state(buttons.buttons[0], NULL) == CLICKED) *action = START_GAME;
    else if (get_button_state(buttons.buttons[1], NULL) == CLICKED) *action = GO_TO_SETTINGS;
    else if (get_button_state(buttons.buttons[2], NULL) == CLICKED) *action = QUIT_GAME;
    else *action = EMPTY;
}
