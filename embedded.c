#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void* taskA( void* args);
void* taskB( void* args);

int main(int argc, char *argv[]) {
	pthread_attr_t attrA;
	pthread_attr_t attrB;
	pthread_t idA;
	pthread_t idB;
	int ret;
	ret = pthread_attr_init( &attrA );
	if (ret != 0) {
		printf("pthread_attr_init: %s\n", strerror(ret));
		exit(-1);
	}
	ret = pthread_attr_init( &attrB );
	if (ret != 0) {
		printf("pthread_attr_init: %s\n", strerror(ret));
		exit(-1);
	}
	pthread_t returnA;
	pthread_t returnB;
	ret = pthread_create( &idA, &attrA, &taskA, NULL );
	if (ret != 0) {
		printf("pthread_create: %s\n", strerror(ret));
		exit(-1);
	}
	ret = pthread_create( &idB, &attrB, &taskB, NULL );
	if (ret != 0) {
		printf("pthread_create: %s\n", strerror(ret));
		exit(-1);
	}
	ret = pthread_join( idA, (void**) &returnA );
	if (ret != 0) {
		printf("pthread_join: %s\n", strerror(ret));
		exit(-1);
	}
	ret = pthread_join( idB, (void**) &returnB );
	if (ret != 0) {
		printf("pthread_join: %s\n", strerror(ret));
		exit(-1);
	}
	printf("ID A: %d / %d; ID B: %d / %d\n", idA, returnA, idB, returnB);
	if (idA != returnA || idB != returnB) {
		printf("Mismatched ID's, something's wrong...");
		exit(-1);
	}
	return EXIT_SUCCESS;
}

void* taskA( void* args ) {
	sleep(5);
	pthread_t id = pthread_self();
	printf("I'm thread A! ID = %d\n", id);
	pthread_exit( (void*) id );
}

void* taskB( void* args ) {
	sleep(2);
	pthread_t id = pthread_self();
	printf("I'm thread B! ID = %d\n", id);
	pthread_exit( (void*) id );
}
