all: shell

shell: shell.o functions.o
	gcc shell.o functions.o -o cs345sh

shell.o: shell.c functions.h
	gcc -c shell.c

functions.o: functions.c functions.h
	gcc -c functions.c

clean:
	rm *.o cs345sh
