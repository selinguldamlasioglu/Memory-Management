all: app mem.o app.o
mem.o:   mem.c
	gcc -g -Wall -c  mem.c
app.o:  app.c
	gcc -g -Wall -c app.c
app: app.o mem.o
	gcc -o app mem.o app.o -lm
clean:
	rm -fr *.o *~ app
