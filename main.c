#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <time.h>

#include "draw.h"
#include "fish.h"
#include "sleep.h"


int width;
int height;

int main() {
	srand(time(0));
	initscr();
	// makes cursor invisible
	curs_set(0);
	noecho();
	cbreak();
	nodelay(stdscr, true);
	// takes 2 chars to make one pixel
	width = getmaxx(stdscr) / 2;
	height = getmaxy(stdscr);

	Point center = {width / 2, height / 2};
	Fish fish = fish_make_default(center);

	Point t = {rand() % width, rand() % height};
	fish_target(&fish, t);

	char chlast = ' ';
	bool pause = false;
	while (true) {
		char ch = getch();
		if (ch == 'q') {
			break;
		}
		if (ch == 'p') {
			pause = !pause;
		}

		width = getmaxx(stdscr) / 2;
		height = getmaxy(stdscr);

		// is erase or clear better?
		erase();
		mvprintw(1, 1, "%f", DELTA_TIME);	
		mvprintw(4, 1, "head: %f %f", fish.head->p.x, fish.head->p.y);

		bool there = false;
		if (!pause) {
			there = fish_swim(&fish);
		}
		if (there) {
			t.x = rand() % width;
			t.y = rand() % height;
			fish_target(&fish, t);
		}

		fish_draw(&fish);
		refresh();
		frameSleep();
	}

	curs_set(1);
	endwin();
	return 0;
}
