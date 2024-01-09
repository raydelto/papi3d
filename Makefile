SRC=src/graphics.c \
	src/map.c \
 	src/misc.c \
	src/player.c

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

all: 
	gcc -c $(SRC) $(INCLUDES) $(WARNINGS)
	ls bin>/dev/null||mkdir bin
	mv *.o ./bin
	gcc src/fps.c $(OBJ) $(LIBS) $(INCLUDES) -o bin/main $(WARNINGS)
clean:
	rm -rf *.o fps
	rm -rf bin
