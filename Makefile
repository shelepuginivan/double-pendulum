build: main.o
	mkdir target/
	gcc *.o -lm -o target/double-pendulum

main.o: main.c
	gcc -c main.c

clean:
	rm -rf target *.o
