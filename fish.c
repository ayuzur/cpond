#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "fish.h"
#include "draw.h"
#include "sleep.h"
#include "utils.h"


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

#define DEFAULT_LENGTH 11
#define DEFAULT_RADII 7, 8, 8, 8, 7, 6, 5, 4, 3, 2, 7
#define DEFAULT_FINC 2
#define DEFAULT_FIN_1 3
#define DEFAULT_FIN_2 7
#define DEFAULT_SPEED 15
#define FISH_DEFAULTS DEFAULT_LENGTH, radii, DEFAULT_LENGTH, fins, DEFAULT_FINC, p, 15
Fish fish_make_default(Point p) {
	// radii are copied to their own varibles in fish_make so
	// no need to allocate
	int radii[DEFAULT_LENGTH] = {DEFAULT_RADII};
	int* fins = (int*) checkedCalloc(2, sizeof(int));
	fins[0] = DEFAULT_FIN_1;
	fins[1] = DEFAULT_FIN_2;
	return fish_make(FISH_DEFAULTS);
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


	Point forward = point_sub(fish->target, fish->start);

	//draw_line_p(fish->start, point_add(fish->start, forward));
	//draw_line_p(fish->start, fish->target);

	// TODO verify
	PerpPoints normPathAxis = getScaledPerpPoints(forward, 1);

	Point forwardTranslation = point_scale(normPathAxis.forward, fish->speed * DELTA_TIME);
	fish->inLinePos = point_add(fish->inLinePos, forwardTranslation);

	// DEBUG
	//draw_line_p(fish->start, fish->inLinePos);

	//draw_pixel_p(fish->target);
	//draw_line_p(fish->start, point_add(fish->start, point_scale(normPathAxis.forward, 10)));
	//draw_line_p(fish->start, point_add(fish->start, point_scale(normPathAxis.backward, 10)));
	//draw_line_p(fish->start, point_add(fish->start, point_scale(normPathAxis.right, 10)));
	//draw_line_p(fish->start, point_add(fish->start, point_scale(normPathAxis.left, 10)));


	// END DEBUG
	
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

