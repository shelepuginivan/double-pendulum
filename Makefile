build: main.o system.o
	mkdir -p target/
	gcc *.o -lm -o target/double-pendulum

main.o: main.c
	gcc -c main.c

system.o: system.c system.h
	gcc -c system.c

clean:
	rm -rf target *.o
