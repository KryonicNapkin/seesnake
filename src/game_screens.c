#include "raylib.h"
#include "raymath.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/game.h"
#include "../include/game_screens.h"

void draw_pause_box(Font font, int border_width, game_theme_t theme) {
    const char* text = "PAUSED";

    Vector2 paused_dims = {WIN_WIDTH/3.0f, WIN_HEIGHT/3.0f};
    Vector2 paused_pos = {(WIN_WIDTH-paused_dims.x)/2.0f, (WIN_HEIGHT-paused_dims.y)/2.0f};

    Vector2 text_pos = {
        .x = paused_pos.x+((paused_dims.x-MeasureTextEx(font, text, font.baseSize, 0).x)/2.0f),
        .y = paused_pos.y+((paused_dims.y-MeasureTextEx(font, text, font.baseSize, 0).y)/2.0f),
    };

    const float padding = 20.0f;
    int color_id = 1;
    for (int i = 0; i < 5; ++i) {
        if (i == 0) {
            DrawRectangleV(Vector2AddValue(paused_pos, padding), 
                           Vector2SubtractValue(paused_dims, 2.0f*padding), 
                           theme.colors[color_id]);
        
        } else {
            DrawRectangleV(Vector2AddValue(paused_pos, (float)(i)*border_width), 
                           Vector2SubtractValue(paused_dims, (float)(i)*2.0f*border_width), 
                           theme.colors[color_id]);
        }
        color_id = !color_id;
    }
    DrawTextEx(font, text, text_pos, font.baseSize, 0, theme.colors[COLOR_FOREGROUND]);
}

void draw_end_game_screen(unsigned long score, long play_time_s, game_action_t* ret_action, game_theme_t theme) {
    const char* text1 = "GAME OVER!";
    const char* texts[] = {"RESTART THE GAME", "QUIT GAME", NULL};
    char stats[128];

    const int button_width = 200;
    const int button_height = 50;

    int secs = play_time_s % 60;
    int mins = play_time_s / 60;
    snprintf(stats, sizeof(stats), "PLAYTIME: %d %s and %d %s   SCORE: %lu", mins, 
             (mins == 1) ? "minute" : "minutes", secs, (secs == 1) ? "second" : "seconds", score);

    Ruies_Rect_t button_bounds = {(WIN_WIDTH-button_width)/2.0f, (WIN_HEIGHT-button_height)/2.0f, button_width, button_height};
    Ruies_ButtonGrid_t buttons = make_button_grid(BUTTON_SPACING, (uint32_t)(WIN_HEIGHT-button_height)/2, 1, 2, button_bounds, texts, button_width, 0);

    DrawRectangleV(Vector2Zero(), (Vector2){WIN_WIDTH, WIN_HEIGHT}, theme.colors[COLOR_BACKGROUND]);

    Vector2 text_pos = {
        .x = (WIN_WIDTH-MeasureTextEx(theme.default_font, text1, theme.font_size, 0).x)/2.0f,
        .y = (WIN_HEIGHT-MeasureTextEx(theme.default_font, text1, theme.font_size, 0).y)/3.0f,
    };
    DrawTextEx(theme.default_font, text1, text_pos, theme.font_size, 0, theme.colors[COLOR_FOREGROUND]);

    Vector2 stats_pos = {
        .x = (WIN_WIDTH-MeasureTextEx(theme.default_font, stats, theme.font_size, 0).x)/2.0f,
        .y = (WIN_HEIGHT-MeasureTextEx(theme.default_font, stats, theme.font_size, 0).y)/3.0f*2.0f,
    };
    DrawTextEx(theme.default_font, stats, stats_pos, theme.font_size, 0, theme.colors[COLOR_FOREGROUND]);

    stretch_button_grid_horiz(&buttons, BUTTON_SPACING, WIN_WIDTH-BUTTON_SPACING);
    if (render_button(&buttons.buttons[0], NULL)) {
        if (ret_action != NULL) *ret_action = RESTART_THE_GAME;
    } else if (render_button(&buttons.buttons[1], NULL)) {
        if (ret_action != NULL) *ret_action = QUIT_GAME;
    }
}

void draw_title_screen(game_action_t* action, game_theme_t theme) {
    Ruies_Rect_t button_bounds = {BUTTON_SPACING, 400, 300, 50};
    const char* text[] = {"Start game", "Quit game", NULL};
    Ruies_ButtonGrid_t buttons = make_button_grid(BUTTON_SPACING, WIN_HEIGHT-BUTTON_SPACING-button_bounds.height, 1, 2, button_bounds, text, 150, 0);
    if (check_ruies_error()) {
        TraceLog(LOG_INFO, "Error");
        exit(EXIT_FAILURE);
    }

    const char* title_text = "SeeSnake";
    const char* author_text = "By KryonicNapkin";
    const char* version_text = "VERSION: "GAME_VERSION;

    Vector2 title_text_pos = {
        .x = (WIN_WIDTH-MeasureTextEx(theme.default_font, title_text, theme.font_size, 0).x)/2.0f,
        .y = (WIN_HEIGHT-MeasureTextEx(theme.default_font, title_text, theme.font_size, 0).y)/6.0f,
    };
    Vector2 author_text_pos = {
        .x = (WIN_WIDTH-MeasureTextEx(theme.default_font, author_text, theme.font_size, 0).x)/2.0f,
        .y = (WIN_HEIGHT-MeasureTextEx(theme.default_font, author_text, theme.font_size, 0).y)/2.5f,
    };
    Vector2 version_text_pos = {
        .x = (WIN_WIDTH-((MeasureTextEx(theme.default_font, version_text, theme.font_size, 0).x)+15.0f)),
        .y = (WIN_HEIGHT-((MeasureTextEx(theme.default_font, version_text, theme.font_size, 0).y)+15.0f)),
    };

    DrawRectangleV(NULL_POS, GAMEWIN_DIMS, theme.colors[COLOR_BACKGROUND]);
    DrawTextEx(theme.default_font, title_text, title_text_pos, theme.font_size, 0, theme.colors[COLOR_FOREGROUND]);
    DrawTextEx(theme.default_font, author_text, author_text_pos, theme.font_size, 0, theme.colors[COLOR_FOREGROUND]);
    DrawTextEx(theme.default_font, version_text, version_text_pos, theme.font_size, 0, theme.colors[COLOR_FOREGROUND]);

    stretch_button_grid_horiz(&buttons, BUTTON_SPACING, WIN_WIDTH-BUTTON_SPACING);

    if (render_button(&buttons.buttons[0], NULL)) *action = START_GAME;
    else if (render_button(&buttons.buttons[1], NULL)) *action = QUIT_GAME;
    else *action = EMPTY;
}
