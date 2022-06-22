.PHONY: clean
CC := g++
CCFLAGS := -Wall -std=gnu++17 -ggdb #-O2

main: main.o parser.o
	$(CC) -o $@ $(CCFLAGS) main.o parser.o

main.o: main.cpp parser.h
	$(CC) -c $(CCFLAGS) main.cpp

simulator.o: simulator.cpp simulator.h
	$(CC) -c -ggdb -Wall -std=gnu++17 -O2 simulator.cpp

parser.o: parser.cpp parser.h
	$(CC) -c $(CCFLAGS) parser.cpp

linear_test: linear_test.o linear.o
	$(CC) -o $@ $(CCFLAGS) linear_test.o linear.o

linear_test.o: linear_test.cpp linear.h
	$(CC) -c $(CCFLAGS) linear_test.cpp

linear.o: linear.cpp linear.h
	$(CC) -c $(CCFLAGS) linear.cpp

clean:
	rm -f *.o
