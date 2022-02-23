#CPPFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu++98
all: ringmaster player
ringmaster: ringmaster.cpp socket.h potato.h
	g++ -g -o $@ $< socket.cpp
player: player.cpp socket.h potato.h
	g++ -g -o $@ $< socket.cpp

.PHONY: clean
clean:
	rm -f *.o *~ ringmaster player