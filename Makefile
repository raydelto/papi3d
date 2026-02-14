CXX=g++

SRC=src/graphics.cpp \
	src/map.cpp \
 	src/pfloat.cpp \
	src/player.cpp

OBJ=graphics.o \
	map.o \
	pfloat.o \
	player.o

WARNINGS=-Wall
CPP_VERSION=-std=c++11
FLAGS=-O2

all: 
	$(CXX) -c $(SRC) $(INCLUDES) $(WARNINGS) $(CPP_VERSION) $(FLAGS)
	ls bin>/dev/null||mkdir bin
	mv *.o ./bin
	$(CXX) src/main.cpp bin/*.o $(LIBS) $(INCLUDES) -o bin/main $(WARNINGS) $(CPP_VERSION) $(FLAGS)


UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	LIBS = -lSDL2
	INCLUDES = -I /usr/include
clean:
	rm -rf *.o main
	rm -rf bin

else ifeq ($(UNAME_S),Darwin)

LIBS= -L/opt/homebrew/opt/sdl2/lib \
	  -lsdl2

INCLUDES=-I/opt/homebrew/opt/sdl2/include \
		-I/usr/local/include

clean:
	rm -rf *.o main
	rm -rf bin

else # Windows
	LIBS = -lSDL2 -LC:\msys64\mingw64\lib -mconsole \
	  -lgdi32
	INCLUDES = -IC:/msys64/mingw64/include

clean:
	del *.o
	del main.exe
	rm -r -fo bin

endif
