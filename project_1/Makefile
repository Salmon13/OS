main: main.o scheduler.o myfunctions.o
	gcc -Wall main.o scheduler.o myfunctions.o -o main
main.o: main.c Makefile
	gcc -Wall main.c -c
scheduler.o: scheduler.c scheduler.h Makefile
	gcc -Wall scheduler.c -c
myfunctions.o: myfunctions.c myfunctions.h Makefile
	gcc -Wall myfunctions.c -c
clean:
	rm -rf *o
run:
	sudo ./main
