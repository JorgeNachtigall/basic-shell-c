all: exec

exec: main.o server.o
	gcc -o exec main.o server.o -g
 
main.o: main.c server.h
	gcc -o main.o main.c -c -g -W -Wall -ansi -pedantic
 
server.o: server.c server.h
	gcc -o server.o server.c -c -g -W -Wall -ansi -pedantic
 
clean:
	rm -rf *.o *~ exec