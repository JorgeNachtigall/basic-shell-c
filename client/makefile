all: exec

exec: main.o client.o
	gcc -o exec main.o client.o -g
 
main.o: main.c client.h
	gcc -o main.o main.c -c -g -W -Wall -ansi -pedantic
 
client.o: client.c client.h
	gcc -o client.o client.c -c -g -W -Wall -ansi -pedantic
 
clean:
	rm -rf *.o *~ exec