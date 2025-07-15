#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <locale.h>

#include "draw.h"
#include "fish.h"
#include "sleep.h"
#include "llist.h"
#include "utils.h"
#include "braille.h"

int width;
int height;

int width_in_ch;
int height_in_ch;

enum flagsWithArgs {SET_FISH_COUNT, SET_FPS, NONE};

int main(int argc, char* argv[]) {
	
	// default fish count
	int fishCount = 1;

	enum flagsWithArgs flag = NONE;
	for (int i = 1; i < argc; i++) {
		char* arg = argv[i];
		
		switch (flag) {
			case NONE:
				break;
			case SET_FISH_COUNT:
				if (arg[0] == '-') {
					printf("ERROR: option -c or --count takes number\n");
					exit(1);
				}
				fishCount = atoi(arg);
				flag = NONE;
				break;
			case SET_FPS:
				if (arg[0] == '-') {
					printf("ERROR: option --fps takes number\n");
					exit(1);
				}
				sleep_set(atoi(arg));
				flag = NONE;
				break;
		
		}

		if (flag == NONE) {
			if (arg[0] == '-') {
				if (strcmp(arg, "--braille") == 0 || strcmp(arg, "-b") == 0) {
					braille_flag = true;
				}
				else if (strcmp(arg, "--count") == 0 || strcmp(arg, "-c") == 0) {
					flag = SET_FISH_COUNT;
				}
				else if (strcmp(arg, "--fps") == 0) {
					flag = SET_FPS;
				}
				else {
					printf("ERROR: invalid option \"%s\"\n", arg);
					exit(1);
				}
			}
			//else {
			//	fishCount = atoi(arg);
			//}

		}
	}
	if (flag != NONE) {
		printf("ERROR: missing value for flag\n");
		exit(1);
	}



	srand(time(0));

	setlocale(LC_ALL, "");

	initscr();
	// makes cursor invisible
	curs_set(0);
	noecho();
	cbreak();
	nodelay(stdscr, true);

	width_in_ch = getmaxx(stdscr);
	height_in_ch = getmaxy(stdscr);

	// divide by 2 as it takes 2 chars to make one pixel
	width = width_in_ch / 2;
	height = height_in_ch;
	
	// explicity set to null so llist_add doesn't cause a segfault trying to access
	// its members
	Llist fishList = {NULL, NULL};

	Point center = {width / 2, height / 2};

	if (braille_flag) {
		braille_init_variables(width_in_ch, height_in_ch);
		center.x = braille_grid_width / 2;
		center.y = braille_grid_height / 2;
	}
	
	Point t; 
	// allocating fishCount number of fish and giving them random targets
	for (int i = 0; i < fishCount; i++) {
		Fish* newFish = (Fish*) checkedCalloc(1, sizeof(Fish));
		
		if (!braille_flag) {
			t.x = rand() % width;
			t.y = rand() % height;
		}
		else {
			t.x = rand() % braille_grid_width;
			t.y = rand() % braille_grid_height;
		}
		*newFish = fish_make_default(center);

		fish_target(newFish, t);
		llist_add(&fishList, newFish);
	}
	
	bool pause = false;

	while (true) {
		char ch = getch();
		if (ch == 'q') {
			break;
		}
		if (ch == 'p') {
			pause = !pause;
		}


		width_in_ch = getmaxx(stdscr);
		height_in_ch = getmaxy(stdscr);


		width = width_in_ch / 2;
		height = height_in_ch;
	
		if (braille_flag) {
			braille_update_variables(width_in_ch, height_in_ch);
		}
		

		// is erase or clear better?
		erase();

		for (llist_Node* node = fishList.head; node != NULL; node = node->next) {
			Fish* fish = (Fish*) node->data;

			bool atTarget = false;
			if (!pause) {
				atTarget = fish_swim(fish);
			}
			// choose new random target if reached target
			if (atTarget) {
				if (!braille_flag) {
					t.x = rand() % width;
					t.y = rand() % height;
				}
				else {
					t.x = rand() % braille_grid_width;
					t.y = rand() % braille_grid_height;
				}
				fish_target(fish, t);
			}

			fish_draw(fish);

			if (braille_flag) {
				draw_braille_screen(braille_screen);
			}
		}
		refresh();
		frameSleep();
	}

	// freeing data (fish) stored in linked list
	for (llist_Node* node = fishList.head; node != NULL; node = node->next) {
		Fish* fish = (Fish*) node->data;
		// frees fish allocated data then the fish itself
		fish_free(fish);
	}

	// frees the nodes contained in fishList
	// NOTE do not free fishList itself as it is on the stack
	llist_node_free(fishList.head);


	curs_set(1);
	endwin();
	return 0;
}
