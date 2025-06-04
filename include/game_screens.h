#ifndef GAME_SCREENS_H_
#define GAME_SCREENS_H_

#include "raylib.h"

#define BUTTON_SPACING 100

/* In which menu is user */
typedef enum UserMenuState {
    MAIN_MENU = 0,
    SETTINGS_MENU,
    RUNNING,
    PAUSED,
} menu_state_t;

/* Actions that user can do in specific menu*/
typedef enum UserMenuActions {
    EMPTY = 0,                          /* NONE */
    START_GAME,                         /* In MAIN_MENU */
    QUIT_GAME,                          /* In MAIN_MENU */
    GO_TO_SETTINGS,                     /* In MAIN_MENU */

    PAUSE,                              /* When RUNNING */
    UNPAUSE,                            /* When PAUSED */

    /* TODO: Make settings actions */
    CHANGE_BG_COLOR,                    /* In SETTINGS_MENU */
    CHANGE_FG_COLOR,                    /* In SETTINGS_MENU */
    CHANGE_NUMBER_OF_ROWS,              /* In SETTINGS_MENU */
    CHANGE_NUMBER_OF_COLS,              /* In SETTINGS_MENU */
    CHANGE_FG_SNAKE_HEAD_COLOR,         /* In SETTINGS_MENU */
    CHANGE_FG_SNAKE_FOOD_COLOR,         /* In SETTINGS_MENU */
    CHANGE_FG_SNAKE_FOOD_BORDER_COLOR,  /* In SETTINGS_MENU */
    GO_BACK_TO_MAIN_MENU,               /* In SETTINGS_MENU */
    SAVE_SETTINGS,                      /* In SETTINGS_MENU */
} menu_action_t;

void draw_title_screen(Font title_font, Font author_font, Ruies_ButtonGrid_t buttons, 
                       menu_action_t* action);
void draw_pause_box(Vector2 pos, Vector2 dims, Font font, const char* text, 
                    int border_width);
int draw_end_game_screen(Font font);

#endif /* GAME_SCREENS_H_ */
