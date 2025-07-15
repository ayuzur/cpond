#include "sleep.h"

#ifdef _WIN32
	#include <windows.h>
	#define MILI_IN_SEC 1000
	#define MPF (float) MILI_IN_SEC / FPS 
	#define SPF MPF / MILI_IN_SEC

	int mpf = MPF;
	sleep_set(int n) {
		float mpf == (float) MILI_IN_SEC / n;
	}
	void frameSleep() {
		Sleep(mpf);
	}
#else
	#include <time.h>
	#define NANO_IN_SEC 1000000000
	#define NPF (float) NANO_IN_SEC / FPS
	#define SPF NPF / NANO_IN_SEC

	struct timespec spf = {0, NPF};
	void sleep_set(int n) {
		float npf = (float) NANO_IN_SEC / n;
		spf.tv_nsec = npf;
	}
	void frameSleep() {
		nanosleep(&spf, NULL);
	}
#endif

