#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/neutrino.h>

int clock_ms();
int changeSystemTick(unsigned int microsecs);
int test_clock_ms();

int main(int argc, char *argv[]) {
	changeSystemTick(10);
	test_clock_ms();
	return 0;
}

int test_clock_ms() {
	struct timespec want, is;
	int ret = clock_gettime( CLOCK_REALTIME, &want );
	if (ret != 0) {
		printf("clock_gettime: %s\n", strerror(ret));
		return -1;
	}
	printf("before: %d.%ld\n", want.tv_sec, want.tv_nsec);
	want.tv_sec += 1;
	printf("  want: %d.%ld\n", want.tv_sec, want.tv_nsec);
	int i;
	for (i = 0; i < 1000; i++) {
		ret = clock_ms();
		if (ret != 0) {
			printf("clock_ms: %s\n", strerror(ret));
			return -1;
		}
	}
	ret = clock_gettime( CLOCK_REALTIME, &is );
	if (ret != 0) {
		printf("clock_gettime: %s\n", strerror(ret));
		return -1;
	}
	printf("    is: %d.%ld\n", is.tv_sec, is.tv_nsec);
	return 0;
}

int clock_ms() {
	struct timespec want;
	int ret = clock_gettime( CLOCK_REALTIME, &want );
	if (ret != 0) {
		return ret;
	}
	want.tv_nsec += 1e6;
	if (want.tv_nsec >= 1e9) {
		want.tv_sec += 1;
		want.tv_nsec -= 1e9;
	}
	ret = clock_nanosleep( CLOCK_REALTIME, TIMER_ABSTIME, &want, NULL );
	if (ret != 0) {
		return ret;
	}
	return 0;
}

int changeSystemTick( unsigned int microsecs ){
	struct _clockperiod old, new;
	new.fract = 0l;
	new.nsec = microsecs * 1e3l;
	int ret = ClockPeriod_r( CLOCK_REALTIME, &new, &old, 0 );
	if (ret != 0) {
		printf("ClockPeriod_r: %s\n", strerror(ret));
		return ret;
	}
	printf("old: %lu\nnew: %lu\n", old.nsec, new.nsec);
	return 0;
}
