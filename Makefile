CC=g++

BUILD_DIR := ./build
CFLAGS=-g -Wall -Wextra -lsfml-system -lsfml-window

SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=game

all: build run

build:
	g++ $(SRCS) -o $(EXECUTABLE) -lsfml-graphics -lsfml-system -lsfml-window -lGL 

run:
	./$(EXECUTABLE)

clean:
	rm -rf *.o game
	rm -rf *.d game

SRCS := $(shell find . -type f -name "*.cpp")