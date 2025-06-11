#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

#include "draw.h"
#include "fish.h"
#include "sleep.h"

#define DELTA_TIME (float) 1 / FPS

int width;
int height;

int main() {
	initscr();
	// makes cursor invisible
	curs_set(0);
	noecho();
	//cbreak();
	// takes 2 chars to make one pixel
	width = getmaxx(stdscr) / 2;
	height = getmaxy(stdscr);

	Point center = {width / 2, height / 2};
	Fish fish = fish_make_default(center);

	//printf("center x: %f\n", center.x);
	//printf("center y: %f\n", center.y);
	//printf("FISH\n");
	//printf("head: %p\n", fish.head);
	//for (Seg* cur = fish.head; cur != NULL; cur = cur->next) {
	//	printf("seg: %p\n", cur);
	//}
	//printf("head pos: %f, %f\n", fish.head->p.x, fish.head->p.y);
	//printf("target: %f, %f\n", fish.target.x, fish.target.y);
	//printf("start: %f, %f\n", fish.start.x, fish.start.y);

	//mvprintw(40, 1, "after getch()");
	//fish.target.x = rand() % width;
	//fish.target.y = rand() % height;
	while (true) {
		width = getmaxx(stdscr) / 2;
		height = getmaxy(stdscr);
		// may want erase instead
		erase();
		mvprintw(1, 1, "%f", DELTA_TIME);
		bool there = fish_move_towards(&fish, fish.target, fish.speed * DELTA_TIME);
		if (there) {
			fish.target.x = rand() % width;
			fish.target.y = rand() % height;

		}
		//Point curPos = fish.head->p;
		//fish_move(&fish, curPos);
		fish_draw(&fish);
		refresh();
		frameSleep();
	}
	// makes cursor visible
	
	curs_set(1);
	endwin();
	return 0;
}
