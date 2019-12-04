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
Hier wird zuerst die Funktion changeSystemTick aus Aufgabe 2 genutzt, um eine präzisere Clock zu bekommen. Danach werden die benötigten Variablen definiert

## Telnet remote shell
+ Mit putty eine telnet verbindung zum beagle bone aufbauen (IP 192.168.8.2)
+ mit root anmelden 
+ $PATH ausgeben 
+ cd /proc/boot
+ mit ls alle executables ausgeben lassen 
+ vergleich mit doku 
+ ->pidin gibt die Prioritäten aller laufenden Prozesse aus


