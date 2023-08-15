CC=g++

BUILD_DIR := ./build
CFLAGS=-g -Wall -Wextra -lsfml-system -lsfml-window

SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=sort_algorithms

all: build run

build:
	g++ $(SRCS) -o $(EXECUTABLE) -lsfml-graphics -lsfml-system -lsfml-window -lGL 

run:
	./$(EXECUTABLE)

clean:
	rm -rf *.o $(EXECUTABLE)
	rm -rf *.d $(EXECUTABLE)

SRCS := $(shell find . -type f -name "*.cpp")