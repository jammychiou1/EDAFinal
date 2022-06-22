.PHONY: all clean

CC = g++


main: main.o parser.o simulator.o
	$(CC) -o $@ -ggdb -Wall -std=gnu++17 -O2 main.o parser.o simulator.o

main.o: main.cpp parser.h
	$(CC) -c -ggdb -Wall -std=gnu++17 -O2 main.cpp

parser.o: parser.cpp parser.h
	$(CC) -c -ggdb -Wall -std=gnu++17 -O2 parser.cpp

simulator.o: simulator.cpp simulator.h
	$(CC) -c -ggdb -Wall -std=gnu++17 -O2 simulator.cpp

clean:
	rm -f *.o

#CC = g++


