#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "draw.h"
#include "utils.h"

int braille_grid_width;
int braille_grid_height;
bool braille_flag = false;
wchar_t* braille_screen = NULL;
int braille_nums[8] = {0x1, 0x8, 0x2, 0x10, 0x4, 0x20, 0x40, 0x80};
const int BRAILLE_CHAR_W = 2;
const int BRAILLE_CHAR_H = 4;
const int BRAILLE_EMPTY = 0x2800;

static int width_in_chars;
static int height_in_chars;
static int prev_width_in_chars;
static int prev_height_in_chars;


void braille_init_variables(int width, int height) {
	width_in_chars = width;
	height_in_chars = height;
	braille_grid_width = width_in_chars * BRAILLE_CHAR_W;
	braille_grid_height = height_in_chars * BRAILLE_CHAR_H;

	int byte_count = width_in_chars * height_in_chars;
	braille_screen = (wchar_t*) checkedCalloc(byte_count, sizeof(wchar_t));
}

void braille_update_variables(int width, int height) {
	prev_width_in_chars = width_in_chars;
	prev_height_in_chars = height_in_chars;

	width_in_chars = width;
	height_in_chars = height;

	braille_grid_width = width_in_chars * 2;
	braille_grid_height = height_in_chars * 4;

	bool isWidthDiff = prev_width_in_chars != width_in_chars;
	bool isHeightDiff = prev_height_in_chars != height_in_chars; 

	bool isZero = width_in_chars == 0 || height_in_chars == 0;

	int screen_size = width_in_chars * height_in_chars;

	if ((isWidthDiff || isHeightDiff) && !isZero) {
		braille_screen = realloc(braille_screen, sizeof(wchar_t) * screen_size);
		if (braille_screen == NULL) {
			endwin();
			printf("ERROR: realloc returned null");
			exit(1);
		}
	}
	for (int i = 0; i < screen_size; i++) {
		// filling with empty braille;
		braille_screen[i] = BRAILLE_EMPTY;
	}
}

void draw_braille_pixel(int x, int y, wchar_t* screen) {

	if (x / BRAILLE_CHAR_W >= width_in_chars || y / BRAILLE_CHAR_H >= height_in_chars || x < 0 || y < 0) {
		return;
	}	

	int char_x_loc = x / BRAILLE_CHAR_W;
	int char_y_loc = y / BRAILLE_CHAR_H;

	int char_index = (char_y_loc * width_in_chars) + char_x_loc;

	int local_x = x % BRAILLE_CHAR_W;
	int local_y = y % BRAILLE_CHAR_H;

	int local_index = local_y * BRAILLE_CHAR_W + local_x;

	wchar_t ch = braille_nums[local_index];

	screen[char_index] |= ch;


}

void draw_braille_screen(wchar_t* screen) {

	for (int y = 0; y < height_in_chars; y++) {
		for (int x = 0; x < width_in_chars; x++) {
			int index = y * width_in_chars + x;
			if (index > braille_grid_width * braille_grid_height -1 || index < 0) {
				printf("ERROR: screen index is out of bounds\n");
				exit(1);
			}
			wchar_t ch = screen[index];
			if (ch != BRAILLE_EMPTY) {
				draw_unicode_ch(x, y, ch);
			}
		}
	}
}
