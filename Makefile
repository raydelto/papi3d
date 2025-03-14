CXX=g++

SRC=src/graphics.cpp \
	src/map.cpp \
 	src/misc.cpp \
	src/player.cpp

OBJ=graphics.o \
	map.o \
	misc.o \
	player.o

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	LIBS = -lSDL2
	INCLUDES = -I /usr/include
clean:
	rm -rf *.o fps
	rm -rf bin

else ifeq ($(UNAME_S),Darwin)

LIBS= -L/opt/homebrew/opt/sdl2/lib \
	  -lsdl2

INCLUDES=-I/opt/homebrew/opt/sdl2/include \
		-I/usr/local/include
else # Windows
	LIBS = -lSDL2 -LC:\msys64\mingw64\lib -mconsole \
	  -lgdi32
	INCLUDES = -IC:/msys64/mingw64/include

clean:
	del *.o
	del fps.exe
	rm -r -fo bin

endif

all: 
	$(CXX) -g -c $(SRC) $(INCLUDES) $(WARNINGS) $(CPP_VERSION)
	$(CXX) -g src/fps.cpp $(OBJ) $(LIBS) $(INCLUDES) -o bin/fps $(WARNINGS) $(CPP_VERSION)


WARNINGS=-Wall

CPP_VERSION=-std=c++11

