#include <stdio.h>
#include <math.h>
#include "utils.h"

void* checkedCalloc(int i, size_t size) {
	void* ptr = calloc(i, size);
	if (ptr == NULL) {
		printf("ERROR: allocation with calloc failed");
		exit(1);
	}
	return ptr;

}

Point rotR(Point p) {
	Point np;
	np.x = p.y;
	np.y = -p.x;
	return np;
}

Point rotL(Point p) {
	Point np;
	np.x = -p.y;
	np.y = p.x;
	return np;
}

PerpPoints getScaledPerpPoints(Point forward, int radius) {
	float dist = sqrtf(forward.x * forward.x + forward.y * forward.y);
	//printf("dist: %f\n", dist);
	float scale = (float) radius / dist;
	forward.x *= scale;
	forward.y *= scale;
	PerpPoints perp = {
		forward,
		rotR(rotR(forward)),
		rotR(forward),
		rotL(forward)
	};
	return perp;
}

Point point_add(Point p1, Point p2) {
	Point p = {p1.x + p2.x, p1.y + p2.y};
	return p;
}
// what was this doing if the return was missing
Point point_sub(Point p1, Point p2) {
	Point p = {p1.x - p2.x, p1.y - p2.y};
	return p;
}

Point point_scale(Point p, float num) {
	Point np;
	np.x = p.x * num;
	np.y = p.y * num;
	return np;
}
bool point_eq(Point p1, Point p2) {
	bool xEq = p1.x == p2.x;
	bool yEq = p1.y == p2.y;
	return xEq && yEq;
}

float point_dist(Point from, Point to) {
	float diffX = to.x - from.x;
	float diffY = to.y - from.y;
	float dist = sqrtf(diffX * diffX + diffY * diffY);
	return dist;
}

Point point_mv_towards(Point from, Point to, float maxDist) {
	float diffX = to.x - from.x;
	float diffY = to.y - from.y;
	float dist = sqrtf(diffX * diffX + diffY * diffY);

	if (dist == 0) {
		return from;
	}
	if (dist <= maxDist) {
		return to;
	}
	float scale = maxDist / dist;
	float scaledX = diffX * scale;
	float scaledY = diffY * scale;

	Point np = {scaledX + from.x, scaledY + from.y};
	return np;
}
