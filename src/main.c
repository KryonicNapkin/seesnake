/* 
 * VERSION HISTORY: 
 *     First version: 1.0.0-beta
 *     Changelog: Initial release of the game
 *
 *     Old version: 1.0.1-beta
 *     Changelog: Changed: Rate of change in the speed of the snake is not depended on that speed
 *                         It is set to the constant game speed of 60FPS
 *                Removed: EVENTS in the include/game_events.h that handled the change of speed from the game events
 *
 *     Current version: 1.1.0-beta
 *     Changelog: Added: You can see your version number in the title screen x
 *                       End screen x
 *                       You can use WASD keys to control your snake x
 *                       CRACOL (CRAzy COLors) Find the button! x
 *                Removed: Text: "DEMO!!!" and "Work in progress..." x
 *                Changed: Updated the ruies.h library x
 *                         Refactor the whole codebase x
 *                         Now using only one font instead of four so the filesize of the executable is smaller x
 *                         The game proportions have been changed significantly x
 *               
 *               And a lot of smaller improvements!!
 *
 */

/* 
 * Started work on the end screen
 */

#include <time.h>

#include "raylib.h"

#define RUIES_USE_CUSTOM_FONT
#define RUIES_IMPLEMENTATION
#include "../lib/ruies.h"

#include "../include/theme.h"
#include "../include/game_screens.h"
#include "../include/grid.h"
#include "../include/snake.h"
#include "../include/game.h"

#include "../include/font.h"

#define STATS_SPACE 25.0f

int main(int argc, char* argv[]) {
    const char* title_text = "SeeSnake";
    InitWindow(WIN_WIDTH, WIN_HEIGHT, title_text);
    SetExitKey(KEY_NULL);

    SetTargetFPS(60);

    game_theme_t theme = get_default_theme();
    game_theme_t orig_theme = theme;

    ruies_load_custom_font(theme.default_font, theme.font_size);
    set_global_style_attr(ATTR_BORDER_WIDTH, ELEM_BORDER_WIDTH);

    SetRandomSeed(time(NULL));

    game_event_t event = 0x00;

    snake_game_grid_t* grid = grid_make(GRID_SIZE, CELL_SIZE, CELL_SPACING_PX, GRID_BORDER_WIDTH);
    snake_t* snake = snake_make(TAIL_LENGHT, SNAKE_VELOCITY, GRID_SIZE);
    game_t* game = NULL;

    game_init(&game, grid, snake, theme);

    #ifdef DEBUG
    TraceLog(LOG_INFO, "Current facing direction: %d", snake->head_direction);
    #endif

    Ruies_Rect_t cracol_bounds = {
        .x = (2.0f*ELEM_BORDER_WIDTH)+1, 
        .y = WIN_HEIGHT-(2.0f*((2.0f*ELEM_BORDER_WIDTH)+1)), 
        .width = (2.0f*ELEM_BORDER_WIDTH)+1, 
        .height = (2.0f*ELEM_BORDER_WIDTH)+1
    };
    Ruies_Button_t cracol_button = make_button(cracol_bounds, "");
    Ruies_ElemState_t cracol_state;
    long val = 0;

    float frame_time = 0.0f; 
    float snake_time_frame = 0.0f; 
    while (!WindowShouldClose() && !game->quit) {
        if (game->state == PAUSED || game->state == RUNNING) {
            if (IsKeyPressed(KEY_SPACE)) {
                if (game->state == PAUSED) game->state = RUNNING;
                else game->state = PAUSED;
            }
        }
        if (game->state == RUNNING) {
            if (IsKeyPressed(KEY_KP_SUBTRACT) && game->snake->velocity-0.5f > 0.0f) {
                game->snake->velocity -= 0.5f;
            }
            else if (IsKeyPressed(KEY_KP_ADD)) game->snake->velocity += 0.5f;
        }

        if (game->state == RUNNING) {
            snake_time_frame += GetFrameTime();
            if (snake_time_frame >= 1.0f) {
                game->game_time_s++;
                snake_time_frame = 0.0f;
            }
            get_game_event(game, &event);
            frame_time += GetFrameTime();
            if (frame_time >= (1.0f/game->snake->velocity)) {
                update_game(&game, event);
                unset_unused_tail_directions(&game->snake, &game->grid);
                event = 0x00;
                frame_time = 0.0f;
            }
        }

        BeginDrawing();
        {
            ClearBackground(game->theme.colors[COLOR_BACKGROUND]);
            if (game->state == MAIN_MENU) {
                draw_title_screen(&game->action, game->theme);
            } else if (game->state == RUNNING) {
                draw_game_title(game->grid->pos, game->grid->dims, title_text, game->theme);
                draw_stats(game, game->theme);

                if (render_button(&cracol_button, &cracol_state)) {
                    change_theme_color(&game->theme, val % NUM_OF_COLORS, game->theme.colors[(val+1) % NUM_OF_COLORS]);
                    val++;
                } else if (cracol_state == FOCUSED && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                    val = 0;
                    game->theme = orig_theme;
                }
                
                game_render(game);
            } else if (game->state == PAUSED) {
                draw_pause_box(game->theme.default_font, GRID_BORDER_WIDTH, game->theme);
            } else if (game->state == END_SCREEN) {
                draw_end_game_screen(game->score, game->game_time_s, &game->action, game->theme);
            }
        }
        EndDrawing();

        update_game_according_to_action(&game);
    }
    CloseWindow();
    game_free(game);
    return 0;
}
