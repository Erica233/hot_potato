#include "ringmaster.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Usage: program <port_num> <num_players> <num_hops>\n";
        return EXIT_FAILURE;
    }
    Ringmaster master(argv[1], argv[2], argv[3]);

    std::cout << "Players = " << master.num_players << std::endl;
    std::cout << "Hops = " << master.num_hops << std::endl;
    std::cout << "port = " << master.port << std::endl;
    return EXIT_SUCCESS;
}

/*
int main(int argc, char **argv) {
    //parse command line argument
    if (argc != 4) {
        std::cerr << "Usage: program <port_num> <num_players> <num_hops>\n";
        return EXIT_FAILURE;
    }
    char * port = argv[1];
    int num_players = atoi(argv[2]);
    if (num_players <= 1) {
        std::cerr << "Invalid num_players\n";
        return EXIT_FAILURE;
    }
    int num_hops = atoi(argv[3]);
    if (num_hops < 0 || num_hops > 512) {
        std::cerr << "Invalid num_hops\n";
        return EXIT_FAILURE;
    }
    std::cout << "Potato Ringmaster\n";
    std::cout << "Players = " << num_players << std::endl;
    std::cout << "Hops = " << num_hops << std::endl;

    int socket_fd = create_server(port);

    //connect ringmaster and each player
    std::vector<std::string> hosts;
    std::vector<uint16_t> ports;
    std::vector<int> fds;
    for (int i = 0; i < num_players; i++) {
        //accept
        struct sockaddr_storage socket_addr;
        socklen_t socket_addr_len = sizeof(socket_addr);
        int client_connect_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
        if (client_connect_fd == -1) {
            std::cerr << "Error: accept() failed\n";
            return EXIT_FAILURE;
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
    //print_vec(num_players, hosts, ports);

    //ring together players
    for (int i = 0; i < num_players; i++) {
        int right_id = i + 1;
        if (right_id == num_players) {
            right_id = 0;
        }
        //std::cout << "right_id: " << right_id << std::endl;
        //std::cout << "right_port (ports[right_id]): " << ports[right_id] << std::endl;
        //std::cout << "right_host (hosts[right_id]): " << hosts[right_id] << std::endl;
        char host_cstr[MAX_HOST_LEN];
        memset(host_cstr, 0, sizeof(host_cstr));
        std::strcpy (host_cstr, hosts[right_id].c_str());
        send(fds[i], host_cstr, sizeof(host_cstr), 0);
        send(fds[i], &ports[right_id], sizeof(ports[right_id]), 0);
    }

    //create potato object
    Potato potato(num_hops);
    //start game
    srand((unsigned int)time(NULL));
    int random = rand() % num_players;
    //std::cout << "random outside of if statement: " << random << std::endl;
    //std::cout << "Ready to start the game, sending potato to player " << random << std::endl;
    //wait for potato back
    if (num_hops == 0) {
        //shut down
    } else {
        send(fds[random], &potato, sizeof(potato), 0);
        std::cout << "Ready to start the game, sending potato to player " << random << std::endl;

        select_read(fds, potato);
        //report results
        std::cout << "Trace of potato: \n";
        //std::cout << "curr_rnd: " << potato.curr_rnd << std::endl;
        for (int i = 0; i < num_hops; i++) {
            if (i == num_hops - 1) {
                std::cout << potato.ids[i] << "\n";
                break;
            }
            std::cout << potato.ids[i] << ",";
        }
    }
    //shut down the game
    for (int i = 0; i < num_players; i++) {
        send(fds[i], &potato, sizeof(potato), 0);
        close(fds[i]);
    }
    close(socket_fd);
    return EXIT_SUCCESS;
}
*/