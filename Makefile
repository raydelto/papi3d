SRC=src/graphics.cpp \
	src/map.cpp \
 	src/misc.cpp \
	src/player.cpp

OBJ=bin/graphics.o \
	bin/map.o \
	bin/misc.o \
	bin/player.o

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS = -SDL2
	INCLUDES = -I /usr/include	
endif
ifeq ($(UNAME_S),Darwin)

LIBS= -L/opt/homebrew/opt/sdl2/lib \
	  -lsdl2

INCLUDES=-I/opt/homebrew/opt/sdl2/include \
		-I/usr/local/include
	
endif

WARNINGS=-Wall

CPP_VERSION=-std=c++11

all: 
	clang++ -g -c $(SRC) $(INCLUDES) $(WARNINGS) $(CPP_VERSION)
	ls bin>/dev/null||mkdir bin
	mv *.o ./bin
	clang++ -g src/fps.cpp $(OBJ) $(LIBS) $(INCLUDES) -o bin/fps $(WARNINGS) $(CPP_VERSION)
clean:
	rm -rf *.o fps
	rm -rf bin
