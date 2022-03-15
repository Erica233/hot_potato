#include "ringmaster.h"

//connect ringmaster and each player
void Ringmaster::connect_players() {
    socket_fd = create_server(port);

    for (int i = 0; i < num_players; i++) {
        //accept
        struct sockaddr_storage socket_addr;
        socklen_t socket_addr_len = sizeof(socket_addr);
        int client_connect_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
        if (client_connect_fd == -1) {
            std::cerr << "Error: accept() failed\n";
            exit(EXIT_FAILURE);
        }
        fds.push_back(client_connect_fd);

        send(client_connect_fd, &i, sizeof(i), 0);
        send(client_connect_fd, &num_players, sizeof(num_players), 0);

        char host_cstr[MAX_HOST_LEN];
        memset(host_cstr, 0, sizeof(host_cstr));
        recv(client_connect_fd, &host_cstr, sizeof(host_cstr), MSG_WAITALL);
        std::string host(host_cstr);
        hosts.push_back(host);

        uint16_t port;
        recv(client_connect_fd, &port, sizeof(port), MSG_WAITALL);
        std::cout << "Player " << i << " is ready to play\n";
        ports.push_back(port);
    }
}

void Ringmaster::print_vec() {
    std::cout << "========print_vec()=========\n";
    std::cout << "id     host            port\n";
    for (int i = 0; i < num_players; i++) {
        std::cout << i << "    " << hosts[i] << "      " << ports[i] << std::endl;
    }
    std::cout << std::endl;
}

//ring together players
void Ringmaster::ring_players() {
    for (int i = 0; i < num_players; i++) {
        int right_id = i + 1;
        if (right_id == num_players) {
            right_id = 0;
        }
        char host_cstr[MAX_HOST_LEN];
        memset(host_cstr, 0, sizeof(host_cstr));
        std::strcpy (host_cstr, hosts[right_id].c_str());
        send(fds[i], host_cstr, sizeof(host_cstr), 0);
        send(fds[i], &ports[right_id], sizeof(ports[right_id]), 0);
    }
}

//send potato
void Ringmaster::play_potato() {
    Potato potato(num_hops);
    //start game
    srand((unsigned int)time(NULL) + num_players);
    int random = rand() % num_players;
    //wait for potato back
    if (num_hops == 0) {
        //shut down
    } else {
        send(fds[random], &potato, sizeof(potato), 0);
        std::cout << "Ready to start the game, sending potato to player " << random << std::endl;

        select_read(fds, potato);

        std::cout << "Trace of potato: \n";
        potato.print_trace();
    }
    //send zero potato to indicate the ending
    for (int i = 0; i < num_players; i++) {
        send(fds[i], &potato, sizeof(potato), 0);
    }
}

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Usage: program <port_num> <num_players> <num_hops>\n";
        return EXIT_FAILURE;
    }
    Ringmaster master(argv[1], argv[2], argv[3]);

    //connect ringmaster and each player
    master.connect_players();
    master.ring_players();

    //srand((unsigned int)time(NULL) + master.get_num_players());
    master.play_potato();

    return EXIT_SUCCESS;
}