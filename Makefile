all: dict1 dict2

dict2: dict2.o
	gcc -o dict2 dict2.o

dict2.o: dict2.c
	gcc -g -O0 -c dict2.c

dict1: dict1.o
	gcc -g -O0 -o dict1 dict1.o

dict1.o: dict1.c
	gcc -g -O0 -c dict1.c

clean:
	rm dict1 dict2 *.o

indent:
	indent -kr -nut -ts4 dict1.c dict2.c
