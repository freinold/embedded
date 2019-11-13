#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

sem_t sem_takt;
sem_t sem;
int count = 0;

void* Task1( void* args );
void* Task2( void* args );
int waste_msecs( unsigned int msecs );
void handler();

int main(int argc, char *argv[]) {
	int ret;
	struct itimerspec spec;
	struct sigaction sact;
	struct sched_param sched_params;
	timer_t timer;
	pthread_t id1;
	pthread_t id2;
	pthread_attr_t attr1;
	pthread_attr_t attr2;

	// Initialize Semaphores --------------------------------------------------
	ret = sem_init(&sem, 0, 0);
	if (ret != 0) {
		printf("sem_init: %s\n", strerror(ret));
		exit(-1);
	}
	ret = sem_init(&sem_takt, 0, 0);
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
	sched_params.sched_priority = 12;
	ret = pthread_attr_setschedparam( &attr1, &sched_params );
	if (ret != 0) {
		printf("pthread_attr_setschedparam: %s\n", strerror(ret));
		exit(-1);
	}
	ret = pthread_create( &id1, &attr1, &Task1, (void**) &sem );
	if (ret != 0) {
		printf("pthread_create: %s\n", strerror(ret));
		exit(-1);
	}

	// Initialize attributes of Task2 ----------------------------------------
	sched_params.sched_priority = 11;
	ret = pthread_attr_setschedparam( &attr1, &sched_params );
	if (ret != 0) {
		printf("pthread_attr_setschedparam: %s\n", strerror(ret));
		exit(-1);
	}

	// Run both tasks --------------------------------------------------------
	ret = pthread_create( &id2, &attr1, &Task2, (void**) &sem );
	if (ret != 0) {
		printf("pthread_create: %s\n", strerror(ret));
		exit(-1);
	}

	// Create timer, set time and set handler for SIG_ALARM event ---
	sact.sa_handler = &handler;
	ret = sigaction(SIGALRM, &sact, NULL);
	if (ret != 0) {
		printf("sigaction: %s\n", strerror(ret));
		exit(-1);
	}
	ret = timer_create(CLOCK_REALTIME, NULL, &timer);
	if (ret != 0) {
		printf("timer_create: %s\n", strerror(ret));
		exit(-1);
	}
	ret = timer_gettime(timer, &spec);
	if (ret != 0) {
		printf("timer_gettime: %s\n", strerror(ret));
		exit(-1);
	}
	spec.it_interval.tv_sec = 0;
	spec.it_interval.tv_nsec = 4e6;
	spec.it_value.tv_sec = 0;
	spec.it_value.tv_nsec = 4e6;
	ret = timer_settime(timer, 0, &spec, NULL);
	if (ret != 0) {
		printf("timer_settime: %s\n", strerror(ret));
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
	int ret;
	int count = 0;
	while (1) {
		count++;
		do {
			ret = sem_wait(&sem_takt);
		}
		while (ret != 0 && errno == EINTR);
		if (ret != 0) {
			perror("sem_wait");
		}
		waste_msecs(2);
		if (count % 3 == 0) {
			ret = sem_post(&sem);
			if (ret != 0) {
				printf("sem_post: %s\n", strerror(ret));
				pthread_exit(0);
			}
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

void handler() {
	int ret = sem_post(&sem_takt);
	if (ret != 0) {
		printf("sem_post: %s\n", strerror(ret));
	}
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
