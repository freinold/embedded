#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

sem_t sem;

void* Task1( void* args );
void* Task2( void* args );
int waste_msecs( unsigned int msecs );

int main(int argc, char *argv[]) {
	int ret;
	struct sched_param sched_params;
	pthread_t id1;
	pthread_t id2;
	pthread_attr_t attr1;
	pthread_attr_t attr2;

	// Initialize Semaphore --------------------------------------------------
	ret = sem_init(&sem, 0, 0);
	if (ret != 0) {
		printf("sem_init: %s\n", strerror(ret));
		exit(-1);
	}

	// Initialize attributes of Task1 and set priority to 11 -----------------
	ret = pthread_attr_init( &attr1 );
	if (ret != 0) {
		printf("pthread_attr_init: %s\n", strerror(ret));
		exit(-1);
	}
	ret = pthread_attr_setinheritsched( &attr1, PTHREAD_EXPLICIT_SCHED );
	if (ret != 0) {
		printf("pthread_attr_setinheritsched: %s\n", strerror(ret));
		exit(-1);
	}
	ret = pthread_attr_getschedparam( &attr1, &sched_params );
	if (ret != 0) {
		printf("pthread_attr_getschedparam: %s\n", strerror(ret));
		exit(-1);
	}
	sched_params.sched_priority = 11;
	ret = pthread_attr_setschedparam( &attr1, &sched_params );
	if (ret != 0) {
		printf("pthread_attr_setschedparam: %s\n", strerror(ret));
		exit(-1);
	}

	// Initialize attributes of Task2 ----------------------------------------
	ret = pthread_attr_init( &attr2 );
	if (ret != 0) {
		printf("pthread_attr_init: %s\n", strerror(ret));
		exit(-1);
	}

	// Run both tasks --------------------------------------------------------
	ret = pthread_create( &id1, &attr1, &Task1, (void**) &sem );
	if (ret != 0) {
		printf("pthread_create: %s\n", strerror(ret));
		exit(-1);
	}
	ret = pthread_create( &id2, &attr2, &Task2, (void**) &sem );
	if (ret != 0) {
		printf("pthread_create: %s\n", strerror(ret));
		exit(-1);
	}

	// Join on Task1 ---------------------------------------------------------
	ret = pthread_join( id1, NULL );
	if (ret != 0) {
		printf("pthread_join: %s\n", strerror(ret));
		exit(-1);
	}
	return EXIT_SUCCESS;
}

void* Task1( void* args ) {
	int ret, i;
	while (1) {
		for(i = 0; i < 3; i++) {
			waste_msecs(2);
			ret = usleep(2);
			if (ret != 0) {
				printf("usleep: %s\n", strerror(ret));
				pthread_exit(0);
			}
		}
		ret = sem_post(&sem);
		if (ret != 0) {
			printf("sem_post: %s\n", strerror(ret));
			pthread_exit(0);
		}
	}
	pthread_exit(0);
}

void* Task2( void* args ) {
	int ret;
	while (1) {
		ret = sem_wait(&sem);
		if (ret != 0) {
			printf("sem_wait: %s\n", strerror(ret));
			pthread_exit(0);
		}
		waste_msecs(3);
	}
	pthread_exit(0);
}

int waste_msecs( unsigned int msecs ) {
	long long i;
	double x, y, z, p;
	for(i = 0; i < msecs * 5880 ; i++)  {
		x = 999999.99999;
		y = 222.645330;
		z = 0.1143839834;
		p = y / x / z;
	}
	return 0;
}
