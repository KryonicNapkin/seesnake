#include <stdlib.h>

#define FONT_H_PATH "include/fonts/"

#include "raylib.h"
#include "include/game.h"

int main(void) {
    InitWindow(1, 1, "hello");
    Font author_font = LoadFontEx(AUTHOR_FONT_PATH, AUTHOR_FONT_SIZE, NULL, 95);
    ExportFontAsCode(author_font, FONT_H_PATH "author_font.h");
    UnloadFont(author_font);
    
    Font title_font = LoadFontEx(TITLE_FONT_PATH, TITLE_FONT_SIZE, NULL, 95);
    ExportFontAsCode(title_font, FONT_H_PATH "title_font.h");
    UnloadFont(title_font);

    Font elem_font = LoadFontEx(ELEM_FONT_PATH, ELEM_FONT_SIZE, NULL, 95);
    ExportFontAsCode(elem_font, FONT_H_PATH "elem_font.h");
    UnloadFont(elem_font);

    Font font = LoadFontEx(FONT_PATH, FONT_SIZE, NULL, 95);
    ExportFontAsCode(font, FONT_H_PATH "font.h");
    UnloadFont(font);
    CloseWindow();
    return 0;
}
