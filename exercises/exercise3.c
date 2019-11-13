#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

int waste_msecs( unsigned int msecs );
int measure_waste_msecs();
int changeSystemTick( unsigned int microsecs );
void* thread_waste_msecs();

int main(int argc, char *argv[]) {
	//To get a more precise clock.
	changeSystemTick(1e3);
	pthread_attr_t attr;
	pthread_t id;
	struct sched_param sched_params;
	int ret;
	ret = pthread_attr_init( &attr );
	if (ret != 0) {
		printf("pthread_attr_init: %s\n", strerror(ret));
		exit(-1);
	}
	ret = pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );
	if (ret != 0) {
		printf("pthread_attr_setinheritsched: %s\n", strerror(ret));
		exit(-1);
	}
	ret = pthread_attr_getschedparam( &attr, &sched_params );
	if (ret != 0) {
		printf("pthread_attr_getschedparam: %s\n", strerror(ret));
		exit(-1);
	}
	printf("standard-prio: %d\n", sched_params.sched_priority);
	sched_params.sched_priority = 255;
	ret = pthread_attr_setschedparam( &attr, &sched_params );
	if (ret != 0) {
		printf("pthread_attr_setschedparam: %s\n", strerror(ret));
		exit(-1);
	}
	printf("set prio to: %d\n", sched_params.sched_priority);
	ret = pthread_create( &id, &attr, &thread_waste_msecs, NULL );
	if (ret != 0) {
		printf("pthread_create: %s\n", strerror(ret));
		exit(-1);
	}
	ret = pthread_join( id, NULL );
	if (ret != 0) {
		printf("pthread_join: %s\n", strerror(ret));
		exit(-1);
	}
	printf("Low prio thread:\n");
	int i;
	for(i = 0; i < 10; i++) {
		measure_waste_msecs();
	}
	return 0;
}

void* thread_waste_msecs( void* args ) {
	pthread_t id = pthread_self();
	struct sched_param my_params;
	int ret;
	ret = pthread_getschedparam( id, NULL, &my_params );
	if (ret != 0) {
		printf("pthread_getschedparam: %s\n", strerror(ret));
		exit(-1);
	}
	printf("I'm the waste thread, my priority is: %d\n", my_params.sched_priority);
	int i;
	for(i = 0; i < 10; i++) {
		measure_waste_msecs();
		usleep(10000);
	}
	pthread_exit(0);
}

int waste_msecs( unsigned int msecs ) {
	long long i;
	double x, y, z, p;
	for(i = 0; i < msecs * 5881 ; i++)  {
		x = 999999.99999;
		y = 222.645330;
		z = 0.1143839834;
		p = y / x / z;
	}
	return 0;
}

int measure_waste_msecs() {
	struct timespec before, after;
	int ret = clock_gettime( CLOCK_REALTIME, &before );
	if (ret != 0) {
		printf("clock_gettime: %s\n", strerror(ret));
		return -1;
	}
	waste_msecs(1e3);
	ret = clock_gettime( CLOCK_REALTIME, &after );
	if (ret != 0) {
		printf("clock_gettime: %s\n", strerror(ret));
		return -1;
	}
	long long nanosecs;
	nanosecs = after.tv_nsec - before.tv_nsec;
	nanosecs += 1e9l * (after.tv_sec - before.tv_sec);
	printf("%lld\n", nanosecs);
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
