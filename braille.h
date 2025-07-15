#ifndef BRAILLE_H
#define BRAILLE_H

extern int braille_grid_width;
extern int braille_grid_height;
extern bool braille_flag;
extern wchar_t* braille_screen;
extern int braille_nums[8];
extern const int BRAILLE_CHAR_W;
extern const int BRAILLE_CHAR_H;
extern const int BRAILLE_EMPTY;

void braille_init_variables(int width, int height);
void braille_update_variables(int width, int height);
void draw_braille_pixel(int x, int y, wchar_t* screen);
void draw_braille_screen(wchar_t* screen);

#endif
