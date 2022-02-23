#ifndef __RINGMASTER_H__
#define __RINGMASTER_H__

#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <vector>

#include "potato.h"
#include "socket.cpp"

#define MAX_NUM_HOPS 512

class Ringmaster {
public:
    char * port;
    int num_players;
    int num_hops;
    int socket_fd;
    std::vector<int> fds;
    std::vector<std::string> hosts;
    std::vector<uint16_t> ports;

public:
    Ringmaster(char * port_num, char * n_players, char * n_hops): port(port_num) {
        num_players = atoi(n_players);
        if (num_players <= 1) {
            std::cerr << "Invalid num_players\n";
            exit(EXIT_FAILURE);
        }
        num_hops = atoi(n_hops);
        if (num_hops < 0 || num_hops > MAX_NUM_HOPS) {
            std::cerr << "Invalid num_hops\n";
            exit(EXIT_FAILURE);
        }
        std::cout << "Potato Ringmaster\n";
        std::cout << "Players = " << num_players << std::endl;
        std::cout << "Hops = " << num_hops << std::endl;
    }
    void connect_players();
    void ring_players();
    void play_potato();
    void print_vec();
    ~Ringmaster() {
        for (int i = 0; i < num_players; i++) {
            //send(fds[i], &potato, sizeof(potato), 0);
            close(fds[i]);
        }
        close(socket_fd);
    }
};

#endif
