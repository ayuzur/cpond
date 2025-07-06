#ifndef FISH_H
#define FISH_H

#include <stdbool.h>

// TODO move point to own file and include that instead
#include "draw.h"

// A segment of the fish body
typedef struct Seg {
	Point p;
	int radius;
	struct Seg* next;
} Seg;

typedef struct Fish {
	// The first segment making up the linked list of segments that is
	// the fish body
	Seg* head;
	// The position the fish is moving towards
	Point target;
	// The position the fish was when the target was chosen
	Point start;
	// int array with fin locations
	// the fin locations are index locations of segments
	int* fins;
	// the number of pairs of fins aka the size of the above array
	int finCount;
	float speed;
	// I believe this is the point representing the fish along the straight
	// path from "start" to "target". If I can remember correctly this 
	// is used to tell when the fish has reached the target, since if I used
	// the real fish position it would be off due to the swimming causing it
	// to not travel to "target" in a straight line but rather a wave.
	Point inLinePos;
} Fish;


// makes fish by linking "length" number of segments each with given "radi"
Fish fish_make(int length, int* radi, int size, int* fins, int finCount, Point p, int speed);

// makes default fish at given point
Fish fish_make_default(Point p);

// draws given fish to the screen
// should probably be rewritten
void fish_draw(Fish* fish);

// moves fish head to point p then updates (pulls along) the other linked segements.
void fish_move(Fish* fish, Point p);

// translates fish head instead of moving it directly to a certain point
void fish_translate(Fish* fish, Point p);

// moves fish towards point and return true when on it
bool fish_move_towards(Fish* fish, Point to, float maxDist);

// sets the fish's swim target
void fish_target(Fish* fish, Point target);

// to be called every frame to create swimming motion using sine wave 
// returns true when target has been reached
bool fish_swim(Fish* fish);

// a recursive function intended to free a segment or linked list of segments
void fish_seg_free(Seg* seg);

// a function to hopefully properly free a given fish
void fish_free(Fish* fish);

#endif
