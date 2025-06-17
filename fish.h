#ifndef FISH_H
#define FISH_H

#include <stdbool.h>

// TODO move point to own file and include that instead
#include "draw.h"

typedef struct Seg {
	Point p;
	int radius;
	struct Seg* next;
} Seg;

typedef struct Fish {
	Seg* head;
	Point target;
	Point start;
	// int array with fin locations as index locations of segments
	int* fins;
	int finCount;
	float speed;
	Point inLinePos;
} Fish;


// makes fish by linking length number of segments each with given radi 
Fish fish_make(int length, int* radi, int size, int* fins, int finCount, Point p, int speed);

// makes default fish at given point
Fish fish_make_default(Point p);

// draws given fish to the screen
// should probably be rewritten
void fish_draw(Fish* fish);

// moves fish head to point p then updates the other linekd segements
void fish_move(Fish* fish, Point p);

void fish_translate(Fish* fish, Point p);

// moves fish towards point and return true when on it
bool fish_move_towards(Fish* fish, Point to, float maxDist);

// sets the fish's swim target
void fish_target(Fish* fish, Point target);

// to be called every frame to create swimming motion using sin wave 
// returns true when target has been reached
bool fish_swim(Fish* fish);

void fish_seg_free(Seg* seg);

void fish_free(Fish* fish);

#endif
