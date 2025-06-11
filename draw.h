#ifndef DRAW_H
#define DRAW_H

typedef struct Point {
	float x;
	float y;
} Point;

// draws 2 characters to form a pixel at given location
void draw_pixelch(int x, int y, char* ch);

// draws "pixel" using default character
void draw_pixel(int x, int y);

void draw_line(int x1, int y1, int x2, int y2);

// draw lines from point to point
void draw_line_p(Point p1, Point p2);

#endif
