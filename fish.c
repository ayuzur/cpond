#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "fish.h"
#include "draw.h"
#include "sleep.h"

void* checkedCalloc(int i, size_t size) {
	void* ptr = calloc(i, size);
	if (ptr == NULL) {
		printf("ERROR: allocation with calloc failed");
		exit(1);
	}
	return ptr;

}

// TODO MOVE TO SEPARATE FILE
typedef struct PerpPoints {
	Point forward;
	Point backward;
	Point right;
	Point left;
} PerpPoints;

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

Point point_sub(Point p1, Point p2) {
	Point p = {p1.x - p2.x, p1.y - p2.y};
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
// END MOVE

// TODO allocate the int* instead of storing on stack
Fish fish_make(int length, int* radi, int size, int* fins, int finCount, Point p, int speed) {
	if (length != size) {
		printf("ERROR: radi in fish_make should be the same length as the int length\n");
		exit(1);
	}

	Seg* head = (Seg*) checkedCalloc(1, sizeof(Seg));
	head->radius = radi[0];
	head->p = p;

	Seg* cur = head;
	for (int i = 1; i < length; i++) {
		cur->next = (Seg*) checkedCalloc(1, sizeof(Seg));
		cur = cur->next;
		cur->radius = radi[i];
		cur->p = p;
	}
	//cur->next = 
	Point d = {0, 0};
	Fish fish = {head, d, d, fins, finCount, speed};
	return fish;
}

Fish fish_make_default(Point p) {
	int radi[11] = {7, 8, 8, 8, 7, 6, 5, 4, 3, 2, 7};
	int* fins = (int*) checkedCalloc(2, sizeof(int));
	fins[0] = 3;
	fins[1] = 7;
	return fish_make(11, radi, 11, fins, 2, p, 15);
}

void fish_draw(Fish* fish) {
	Seg* head = fish->head;

	Seg* cur;
	Seg* prev = NULL;

	PerpPoints prevDirVec;
	int counter = 0;
	for (cur = head; cur != NULL; cur = cur->next) {
		// spine lines
		if (prev != NULL) {
			
			//draw_set_color(GREEN);
			draw_line(cur->p.x, cur->p.y, prev->p.x, prev->p.y);
			//draw_set_color(RESET);	
			
			Point forwardVec = {prev->p.x - cur->p.x, prev->p.y - cur->p.y};
			PerpPoints dirVec = getScaledPerpPoints(forwardVec, cur->radius);
			
			if (prev == head) {
				Point curLeft = point_add(cur->p, dirVec.left);
				Point curRight = point_add(cur->p, dirVec.right);
				Point prevForward = point_add(prev->p, prevDirVec.forward);
				Point prevLeft = point_add(prev->p, prevDirVec.left);
				Point prevRight = point_add(prev->p, prevDirVec.right);

				//draw_set_color(YELLOW);
				//draw_pixel_p(curLeft);
				//draw_pixel_p(curRight);
				//draw_pixel_p(prevForward);

				//draw_set_color(RED);
				draw_line_p(curLeft, prevLeft);
				draw_line_p(curRight, prevRight);
				draw_line_p(prevLeft, prevForward);
				draw_line_p(prevRight, prevForward);
				//draw_set_color(RESET);
			} else {
				Point curLeft = point_add(cur->p, dirVec.left);
				Point curRight = point_add(cur->p, dirVec.right);
				Point prevLeft = point_add(prev->p, prevDirVec.left);
				Point prevRight = point_add(prev->p, prevDirVec.right);

				//draw_set_color(RED);
				draw_line_p(curLeft, prevLeft);
				draw_line_p(curRight, prevRight);
				
				// at tail
				if (cur->next == NULL) {
					Point curBack = point_add(cur->p, dirVec.backward);

					draw_line_p(curLeft, curBack);
					draw_line_p(curRight, curBack);
				}
				// fins
				for (int i = 0; i < fish->finCount; i++) {
					if (counter == fish->fins[i] - 1) {
						Point finR = point_scale(dirVec.right, 1.5);
						Point finL = point_scale(dirVec.left, 1.5);
						finR = point_add(cur->p, finR);
						finL = point_add(cur->p, finL);
						draw_line_p(finR, curRight);
						draw_line_p(finL, curLeft);
						draw_line_p(finR, prevRight);
						draw_line_p(finL, prevLeft);
					}
				}
				//draw_set_color(RESET);
			}
			
			prevDirVec = dirVec;
		}		
		else if (cur == head) {
			Point forwardVec;
			forwardVec.x = cur->p.x - cur->next->p.x;
			forwardVec.y = cur->p.y - cur->next->p.y;
			prevDirVec = getScaledPerpPoints(forwardVec, cur->radius);
		}
		prev = cur;
		counter++;
	}
	

	// point dots
	for (cur = head; cur != NULL; cur = cur->next) {	
		draw_pixel(cur->p.x, cur->p.y);
	}

}

void fish_update_seg(Seg* cur, Seg* prev) {
	// what plus cur x == prev x
	float diffX = prev->p.x - cur->p.x;
	float diffY = prev->p.y - cur->p.y;
	float dist = sqrtf(diffX * diffX + diffY * diffY);

	int combinedRadi = cur->radius;
	// if seg circles dont interlap
	if (dist > combinedRadi) {
		// find scale that dist is equal to combinedRadi
		float scale = combinedRadi / dist;

		// scale diffs
		float scaledDiffX = diffX * scale;
		float scaledDiffY = diffY * scale;

		// find difference betwen scaled and current

		float diffDiffX = diffX - scaledDiffX;
		float diffDiffY = diffY - scaledDiffY;

		cur->p.x += diffDiffX;
		cur->p.y += diffDiffY;

	}
}

void fish_update_segs(Seg* cur, Seg* prev) {
	if (cur == NULL) {
		return;
	}
	if (prev == NULL) { // if prev null this should be first iteration
		fish_update_segs(cur->next, cur);
		return;
	}
	else {
		fish_update_seg(cur, prev);
		fish_update_segs(cur->next, cur);
	}
}

void fish_update(Fish* fish) {
	fish_update_segs(fish->head, NULL);
}

void fish_move(Fish* fish, Point p) {
	fish->head->p = p;
	fish_update(fish);
}

bool fish_move_towards(Fish* fish, Point to, float maxDist) {
	fish->head->p = point_mv_towards(fish->head->p, to, maxDist);
	fish_update(fish);

	return point_eq(fish->head->p, to);

}

void fish_target(Fish* fish, Point target) {
	fish->target = target;
	fish->start = fish->head->p;
	fish->inLinePos = fish->start;
}

void fish_translate(Fish* fish, Point p) {
	fish->head->p = point_add(fish->head->p, p);
	fish_update(fish);
}

bool fish_swim(Fish* fish) {
	// TODO have this be calculated when targeting
	float total = point_dist(fish->start, fish->target);
	float traveled = point_dist(fish->inLinePos, fish->start);
	float remaining = total - traveled;
	if (remaining < fish->speed * DELTA_TIME) {
		return true;
	}

	draw_line_p(fish->start, fish->inLinePos);

	draw_pixel_p(fish->target);

	Point forward = point_sub(fish->target, fish->start);
	PerpPoints normPathAxis = getScaledPerpPoints(forward, 1);

	Point forwardTranslation = point_scale(normPathAxis.backward, fish->speed * DELTA_TIME);
	fish->inLinePos = point_add(fish->inLinePos, forwardTranslation);

	traveled = point_dist(fish->inLinePos, fish->start);

	float period = 40;
	float height = 15;	
	float displacement = sin((traveled * M_PI) / period) * height;

	Point horizontalTranslation = point_scale(normPathAxis.left, displacement);

	Point newPos = point_add(horizontalTranslation, fish->inLinePos);
	fish_move(fish, newPos);
	//fish_translate(fish, translation);
	
	return false;
}

