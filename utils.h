#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
// replace wth point.h
#include "draw.h"

// stores vectors that represent the given directions
typedef struct PerpPoints {
	Point forward;
	Point backward;
	Point right;
	Point left;
} PerpPoints;

// wrapper around calloc so that it autmatically checks if allocation 
// failed
void* checkedCalloc(int i, size_t size);

// rotates given point 90 degrees to the right assuming 0,0 as origin
Point rotR(Point p);

// rotates given point 90 degrees to the left assuming 0,0 as origin
Point rotL(Point p);

// given a forward vector, gets corresponding back, right, and left vectors
// scaled to length "radius"
PerpPoints getScaledPerpPoints(Point forward, int radius);

// add two points
Point point_add(Point p1, Point p2);

// subtract two points
Point point_sub(Point p1, Point p2);

// scale point by num
Point point_scale(Point p, float num);

// check if points are equal
bool point_eq(Point p1, Point p2);

// get distance between points
float point_dist(Point from, Point to);

// returns a point "maxDist" units towards "to" from "from". if distance is 
// shorter than maxDist just returns "to"
Point point_mv_towards(Point from, Point to, float maxDist);

#endif
