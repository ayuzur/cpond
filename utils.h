#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
// replace wth point.h
#include "draw.h"

typedef struct PerpPoints {
	Point forward;
	Point backward;
	Point right;
	Point left;
} PerpPoints;

void* checkedCalloc(int i, size_t size);

Point rotR(Point p);

Point rotL(Point p);

PerpPoints getScaledPerpPoints(Point forward, int radius);

Point point_add(Point p1, Point p2);

Point point_sub(Point p1, Point p2);

Point point_scale(Point p, float num);

bool point_eq(Point p1, Point p2);

float point_dist(Point from, Point to);

Point point_mv_towards(Point from, Point to, float maxDist);

#endif
