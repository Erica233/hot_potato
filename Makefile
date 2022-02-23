#CPPFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu++98
all: ringmaster player
ringmaster: ringmaster.cpp socket.cpp potato.h
	g++ -g -o $@ $<
player: player.cpp socket.cpp potato.h
	g++ -g -o $@ $<

.PHONY: clean
clean:
	rm -f *.o *~ ringmaster player