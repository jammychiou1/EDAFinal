main: main.o parser.o
	g++ -o $@ -g -Wall -std=gnu++17 -O2 main.o parser.o

main.o: main.cpp parser.h
	g++ -c -g -Wall -std=gnu++17 -O2 main.cpp

parser.o: parser.cpp parser.h
	g++ -c -g -Wall -std=gnu++17 -O2 parser.cpp

clean:
	rm -f *.o

#CC = g++


