.PHONY: all clean

$(shell mkdir -p obj)

SRCDIR := src
OBJDIR := obj
HEADERDIR := include

HEADERS := $(wildcard $(HEADERDIR)/*.h)
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
TARGET := test

CC := g++
CCFLAGS := -Wall -std=gnu++17 -O2

all: $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) $(CCFLAGS) -c -o $@ $< -I $(HEADERDIR)

test: test.cpp $(OBJS) $(HEADERS)
	$(CC) $(CCFLAGS) -o $@ $< $(OBJS) -I $(HEADERDIR)

clean:
	rm -f $(TARGET) $(OBJS)

