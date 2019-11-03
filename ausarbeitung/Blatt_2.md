# Blatt 2
## a)
### Code:
```c
int clock_ms() {
 	struct timespec want;
 	int ret = clock_gettime( CLOCK_REALTIME, &want );
 	if (ret != 0) {
 		return ret;
 	}
 	want.tv_nsec += 1e6;
 	if (want.tv_nsec >= 1e9) { //IF Nanosekundenüberlauf/Sekundenwechsel
 		want.tv_sec += 1;
 		want.tv_nsec -= 1e9;
 	}
 	ret = clock_nanosleep( CLOCK_REALTIME, TIMER_ABSTIME, &want, NULL );
 	if (ret != 0) {
 		return ret;
 	}
 	return 0;
 }
 ```
 Die clock_ms Funktion speichert mittels clock_gettime() die aktuelle Zeit in einem timespec struct.
 Auf dieses werden 10^6 Nanosekunden(eine Millisekunde) addiert. Wenn dabei ein "Sekundenübergang" stadtfindet 
 wird dieser anschließend ausgeglichen.
Mit diesem timespec struct als Parameter wird clock_nanosleep aufgerfuen.

#### Überprüfung der Uhr
```c
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
 ```
 In dieser Funktion wird mittels clock_ms() 1000 mal 1ms gewartet und anschließend die akuelle Zeit mit
 einem zuvor ermittelten Zeistempel verglichen. Das Ergebnis war meistens korrekt hatte aber
 gelegentlich geringe Abweichugen. Die Abweichungen kommen warscheinlich von der Ausnahmebehandlung
 von Sekundenübergängen.
     
## b)
### Code:
```c
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

 ```
 Mit changeSystemTick() der Systemtakt auf bis zu minimal 10 Mikrosekunden eingestellt werden. Dafür 
 nutzt die Funktion ClockPeriod_r(). ClockPeriod_r() gibt auserdem den alten Systemtakt aus.
 Dieser wird am Ende der Funktion zusammen mit dem neuen Systemtakt ausgegeben.
 Auswirkungen auf a):
 Bei einer Verlangsamung des Systemtaktes unter den Standardtakt  wird die Uhr um den Faktor der Verlangsamung langsamer.
 Bei einer Erhöung des des Systemtaktes erhöt sich die Genauingkeit der Uhr, diese Funktioniert aber noch wie zuvor.
