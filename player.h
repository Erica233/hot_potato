#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <vector>
#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "socket.h"
#include "potato.h"
#include "player.h"

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

    Potato potato;
    std::vector<int> ids;
    std::vector<int> fds;

public:
    Player(char * host, char * port): {
        master_host = argv[1];
        master_port = argv[2];
    }
};

#endif
