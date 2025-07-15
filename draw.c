#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>

#include "braille.h"
#include "draw.h"

void draw_pixelch(int x, int y, char ch) {
	mvprintw(y, x * 2, "%c%c", ch, ch);
}

void draw_unicode_ch(int x, int y, wchar_t wc) {
	cchar_t wch;
	setcchar(&wch, &wc, 0, 0, NULL);
	mvadd_wch(y, x, &wch);
}

void draw_unicode_pixel(int x, int y, wchar_t wc) {
	cchar_t wch;
	setcchar(&wch, &wc, 0, 0, NULL);
	mvadd_wch(y, x * 2, &wch);
	mvadd_wch(y, (x * 2) + 1, &wch);
}


void draw_pixel(int x, int y) {

	if (braille_flag) {
		draw_braille_pixel(x, y, braille_screen);
	}
	else {
		draw_pixelch(x, y, '#');
	}
}

void draw_pixel_p(Point p) {
	draw_pixel(p.x, p.y);
}

int sign(int i) {
	if (i < 0) {
		return -1;
	}
	else if (i > 0) {
		return 1;
	}
	else {
		return 0;
	}
}
void draw_line(int x1, int y1, int x2, int y2) {
	if (x1 > x2) {
		int swpX = x1;
		int swpY = y1;
		x1 = x2;
		y1 = y2;
		x2 = swpX;
		y2 = swpY;

	}
	int diffX = x2 - x1;
	int diffY = y2 - y1;

	if (diffX == 0) {
		int signY = sign(diffY);
		for (int i = 0; i <= abs(diffY); i++) {
			draw_pixel(x1, y1 + (signY * i));
		}
		return;
	}

	float slope = (float) diffY / diffX;	

	int previousY = y1;
	for (int i = 0; i <= diffX; i++) {
		int newX = i + x1;
		int newY = i * slope + y1;
		draw_pixel(newX, newY);

		int yDifference = abs(newY - previousY);
		if (yDifference > 1) {
			for (int i = 1; i < yDifference; i++) {
				draw_pixel(newX - 1, previousY + (i * sign(newY - previousY)));
			}
		}
		previousY = newY;
	}

}

void draw_line_p(Point p1, Point p2) {
	draw_line(p1.x, p1.y, p2.x, p2.y);
}
