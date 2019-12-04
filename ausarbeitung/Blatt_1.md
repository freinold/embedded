# Blatt 1 - POSIX threads
## Funktionen
Neben der main werden noch die beiden Funktionen taskA & taskB definiert, die sich jedoch nur in Wartezeit und Textausgabe unterscheiden.
### main()
Folgender Code-Ausschnitt beinhaltet der Übersichtlichkeit halber kein Error-Handling:
```c
int main(int argc, char *argv[]) {
	pthread_attr_t attrA;
	pthread_attr_t attrB;
	pthread_t idA;
	pthread_t idB;
	int ret;
	ret = pthread_attr_init( &attrA );
	ret = pthread_attr_init( &attrB );
	pthread_t returnA;
	pthread_t returnB;
	ret = pthread_create( &idA, &attrA, &taskA, NULL );
	ret = pthread_create( &idB, &attrB, &taskB, NULL );
	ret = pthread_join( idA, (void**) &returnA );
	ret = pthread_join( idB, (void**) &returnB );
	printf("ID A: %d / %d; ID B: %d / %d\n", idA, returnA, idB, returnB);
	return EXIT_SUCCESS;
```
Nach Definition der Benötigten Variablen werden zuerst jeweils die Thread-Attribute initialisiert. Danach werden die Threads erstellt, wobei einfach die Speicheradresse der Tasks als Startadresse mitgegeben wird. In die mitgegebene Speicheradresse der ID wird diese vom OS geschrieben, damit der Thread danach wieder referenziert werden kann. Zuletzt wird auf beide Threads gejoint, um mit der weiteren Fortführung der main bis zum Ende der Threads zu warten, und der return-Wert des Thread in die jeweilige Variable geschrieben.
### taskA() / taskB()
```c
void* taskA( void* args ) {
	sleep(5);
	pthread_t id = pthread_self();
	printf("I'm thread A! ID = %d\n", id);
	pthread_exit( (void*) id );
}
```
Der Task schläft für eine bestimmte Zeit (entweder 2 oder 5 Sekunden), holt sich dann durch einen System Call seine ID, gibt diese zuerst in der Konsole aus und schließt sich dann, wobei er seine ID noch mit zurück gibt.
