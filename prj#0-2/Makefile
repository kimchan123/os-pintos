testlib : main.o list.o hash.o bitmap.o hex_dump.o
	gcc -o testlib main.o list.o hash.o bitmap.o hex_dump.o

main.o : main.c
	gcc -c main.c

hex_dump.o : hex_dump.c hex_dump.c
	gcc -c hex_dump.c

list.o : list.c list.h
	gcc -c list.c

hash.o : hash.c hash.h
	gcc -c hash.c

bitmap.o : hex_dump.o  bitmap.c bitmap.h limits.h round.h
	gcc -c bitmap.c

clean :
	rm *.o testlib
