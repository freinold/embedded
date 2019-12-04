# Blatt 3 - Verarbeitungszeit
## Funktionen
### main()
Folgender Code-Ausschnitt beinhaltet der Übersichtlichkeit halber kein Error-Handling:
```c
int main(int argc, char *argv[]) {
	//To get a more precise clock.
	changeSystemTick(1e3);
	pthread_attr_t attr;
	pthread_t id;
	struct sched_param sched_params;
	int ret;
	ret = pthread_attr_init( &attr );
	ret = pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );
	ret = pthread_attr_getschedparam( &attr, &sched_params );
	printf("standard-prio: %d\n", sched_params.sched_priority);
	sched_params.sched_priority = 255;
	ret = pthread_attr_setschedparam( &attr, &sched_params );
	printf("set prio to: %d\n", sched_params.sched_priority);
	ret = pthread_create( &id, &attr, &thread_waste_msecs, NULL );
	ret = pthread_join( id, NULL );
	int i;
	for(i = 0; i < 10; i++) {
		measure_waste_msecs();
	}
	return 0;
}
```
Hier wird zuerst die Funktion changeSystemTick aus Aufgabe 2 genutzt, um eine präzisere Clock zu bekommen. Danach werden die benötigten Variablen definiert, das Thread-Attribut initialisiert und die Schedule-Parameter in diesem verändert, in dem die Priorität auf 255 gesetzt wird, um zu verhindern, dass der Thread durch andere Prozesse unterbrochen wird. Danach wird der Thread zum Verschwenden der Millisekunden thread_waste_msecs() gestarted und der Hauptprozess wartet auf ein Rückkehren dieses. Nach der Rückkehr wird die Messung mit measure_waste_msecs() noch 10 mal mit niedriger Prio des Hauptprozesses durchgeführt.
### thread_waste_msecs()
Folgender Code-Ausschnitt beinhaltet der Übersichtlichkeit halber kein Error-Handling:
```c
void* thread_waste_msecs( void* args ) {
	pthread_t id = pthread_self();
	struct sched_param my_params;
	int ret;
	ret = pthread_getschedparam( id, NULL, &my_params );
	printf("I'm the waste thread, my priority is: %d\n", my_params.sched_priority);
	int i;
	for(i = 0; i < 10; i++) {
		measure_waste_msecs();
		usleep(10000);
	}
	pthread_exit(0);
}
```
Der Prozess gibt zuerst noch einmal seine Priorität aus und führt dann 10 mal im Wechsel zuerst die Messung measure_waste_msecs() durch und schläft dann immer kurz mit usleep() 10.000 Mikrosekunden, um den anderen Prozessen Zeit zum Rechnen zu geben.
### measure_waste_msecs()
Folgender Code-Ausschnitt beinhaltet der Übersichtlichkeit halber kein Error-Handling:
```c
int measure_waste_msecs() {
	struct timespec before, after;
	int ret = clock_gettime( CLOCK_REALTIME, &before );
	waste_msecs(1e3);
	ret = clock_gettime( CLOCK_REALTIME, &after );
	long long nanosecs;
	nanosecs = after.tv_nsec - before.tv_nsec;
	nanosecs += 1e9l * (after.tv_sec - before.tv_sec);
	printf("%lld\n", nanosecs);
	return 0;
}
```
Zuerst werden 2 timespec-Variablen definiert, in die vor und nach Aufruf von waste_msecs(1000) durch clock_gettime() befüllt werden. Danach werden die tatsächlich verstrichenen Nanosekunden berechnet und in der Konsole ausgegeben. 
### int waste_msecs( unsigned int msecs )
```c
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
```
Zum Verschwenden der Millisekunden läuft in einer Schleife 5881 mal eine doppelte Division von Fließkommawerten durch.
## telnet Remote-Shell
+ Mit PuTTY eine telnet-Verbindung zum Beagle Bone Black aufbauen (IP 192.168.8.2)
+ als root anmelden 
+ $PATH ausgeben 
+ cd /proc/boot
+ mit ls alle Executables ausgeben lassen 
+ Vergleich mit doku 
+ -> pidin gibt die Prioritäten aller laufender Prozesse aus
