#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <vector>
#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "socket.cpp"
#include "potato.h"

class Player {
private:
    char * master_host;
    char * master_port;
    int socket_fd;  //connect itself with ringmaster
    int num_players;
    int id;
    int left_id;
    int right_id;

    int as_server_fd;  //as a server, connect with left
    int client_connect_fd;

    int as_client_fd;  //as a client, connect with right

    std::vector<int> ids;  //{right_id, left_id}
    std::vector<int> fds;  //{as_client_fd, client_connect_fd, socket_fd})

    void cal_neighbor_id();
    void send_host_port();

public:
    Player(char * host, char * port) {
        master_host = host;
        master_port = port;
    }
    int getId() {return id;}
    void setup();
    void play_potato();
    ~Player() {
        close(socket_fd);
        close(as_server_fd);
        close(as_client_fd);
        close(client_connect_fd);
    }
};

#endif
