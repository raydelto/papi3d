all: fps.o graphics.o map.o player.o misc.o
	gcc graphics.o fps.o map.o player.o misc.o -o fps -lSDL2 -lm
clean:
	rm -rf *.o fps

