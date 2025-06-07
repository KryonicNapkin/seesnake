#ifndef GAME_SCREENS_H_
#define GAME_SCREENS_H_

#include "raylib.h"

#include "theme.h"

#define BUTTON_SPACING 100

/* Game state */
typedef enum GameState {
    MAIN_MENU = 0,
    END_SCREEN,
    RUNNING,
    PAUSED,
} game_state_t;

/* Actions that user can do */
typedef enum Actions {
    EMPTY = 0,                          /* NONE */
    START_GAME,                         /* In MAIN_MENU */
    QUIT_GAME,                          /* In MAIN_MENU/END_SCREEN */
    TO_MAIN_MENU,                       /* When the game is running */

    SHOW_END_SCREEN,                    /* When collision */
    RESTART_THE_GAME,                   /* In END_SCREEN */

    PAUSE,                              /* When RUNNING */
    UNPAUSE,                            /* When PAUSED */
} game_action_t;

void draw_pause_box(Font font, int border_width, game_theme_t theme);
void draw_end_game_screen(unsigned long score, long play_time_s, game_action_t* ret_action, game_theme_t theme);
void draw_title_screen(game_action_t* action, game_theme_t theme);

#endif /* GAME_SCREENS_H_ */
