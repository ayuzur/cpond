#include <stdio.h>
#include <ncurses.h>

#include "draw.h"

int main() {
	initscr();
	// makes cursor invisible
	curs_set(0);

	Point p1 = {10, 10};
	Point p2 = {20, 20};
	draw_line_p(p1, p2);
	getch();

	// makes cursor visible
	curs_set(1);
	endwin();
	return 0;
}
