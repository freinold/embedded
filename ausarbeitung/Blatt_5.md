# Blatt 5
## Code
```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define DEVICE "/dev/leds"

char calculate( char c){
	//reset state of led3, led4
	c = c & 0x03;
	//extract leds
	char led1 = c & 0x01;
	char led2 = c & 0x02;
	//calculate leds
	led2 = led2 >> 1;
	char led3 = led1 ^ led2;
	char led4 = led1 | led2;
	//put all the bits into result
	c =  (led4 << 3) | (led3 << 2) | c;
	return c;

}
int main(int argc, char *argv[]) {
	char led_code = 0x00;
	char c;
	int ret, fd;
	fd = open(DEVICE, O_RDWR);
	if (fd < 0) {
		perror("open");
		exit(-1);
	}
	ret = write( fd, &led_code, sizeof(led_code));
	if (ret < 0) {
		perror("write");
		exit(-1);
	}
	printf("Enter Number of LED (1 or 2) to switch: ");
	while (1) {
		c = getchar();
		if( c == 'q'){
			break;
		}
		printf("\n");
		switch(c){
			case '1':
				led_code = (char) led_code ^ 0x01;
				break;
			case '2':
				led_code = (char) led_code ^ 0x02;
				break;
			case '\n':
				continue;
			default:
				printf("Wrong character entered!\n");
				break;
		}
		led_code = calculate(led_code);
		ret = write( fd, &led_code, sizeof(led_code));
		if (ret < 0) {
			perror("write");
		}
		printf("Enter Number of LED (1 or 2) to switch: ");
	}
	ret = close(fd);
	if (ret < 0) {
		perror("close");
	}
	return 0;
}

```
## Erläuterung:
Um die LEDs des BBB anzusteurn müssen die Entsprechende Bits unter "/dev/leds" gesetzt werden.
Dazu muss mit dem Befehl open() ein  Filedescriptor erstellt weren. Über disen können anschließen mit write()
die Bits in den Speicher geschrieben werden.
Um die ersten zwei steuerbaren Leds zu schalten weden wir eine xor Operation des aktuellen Zustande der Leds mit  "0x01" bzw " 0x02"
an. Dadurch ändert sich nur das Bit an der gewünschten Stelle.
Um die anderen zwei bits zu errechnen extrahieren wir die für die zwei ersten LEDs zuständige bits aus dem aktuellen Registerwert.
Mit den extrahierten Bits errechnen wir das xor und das or bit und schrieben diese anschließend mit oder Operationen wieder in das Statusregister.
Falls das Programm beendet wird  wird der Filedescriptor wieder gecshlossen.
