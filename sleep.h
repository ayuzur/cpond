
#ifndef SLEEP_H
#define SLEEP_H
#define FPS 30

#define DELTA_TIME (float) 1 / FPS

// sleeps for a frame
void sleep_set(int n);
void frameSleep();

#endif
