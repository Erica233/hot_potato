#CPPFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu++98
all: ringmaster player
ringmaster: ringmaster.cpp socket.h potato.h
	g++ -g -o $@ $<
player: player.cpp socket.h potato.h
	g++ -g -o $@ $<

.PHONY: clean
clean:
	rm -f *.o *~ ringmaster player