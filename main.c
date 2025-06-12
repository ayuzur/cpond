#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <time.h>

#include "draw.h"
#include "fish.h"
#include "sleep.h"
#include "llist.h"
#include "utils.h"

int width;
int height;
 
int main(int argc, char* argv[]) {
	
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

	int fishNum = 1;
	
	if (argc > 1) {
		fishNum = atoi(argv[1]);
	}

	// explicity set to null so llist_add doesn't cause a segfault
	Llist fishList = {NULL, NULL};

	Point center = {width / 2, height / 2};
	
	Point t; 
	for (int i = 0; i < fishNum; i++) {
		Fish* newFish = (Fish*) checkedCalloc(1, sizeof(Fish));
		t.x = rand() % width;
		t.y = rand() % height;

		*newFish = fish_make_default(center);
		fish_target(newFish, t);
		llist_add(&fishList, newFish);
	}
	
	//fishList.head = checkedCalloc(1, sizeof(llist_Node));

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

		// macro (expands into for loop that iterates through nodes)
		llist_iterate(fishList) {
			Fish* fish = (Fish*) node->data;

			bool there = false;
			if (!pause) {
				there = fish_swim(fish);
			}
			if (there) {
				t.x = rand() % width;
				t.y = rand() % height;
				fish_target(fish, t);
			}

			fish_draw(fish);
		}
		refresh();
		frameSleep();
	}

	curs_set(1);
	endwin();
	return 0;
}
