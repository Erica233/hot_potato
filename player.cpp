#include "player.h"

//calculate its neighbor's id
void Player::cal_neighbor_id() {
    left_id = id - 1;
    if (left_id < 0) {
        left_id = num_players - 1;
    }
    right_id = id + 1;
    if (right_id == num_players) {
        right_id = 0;
    }
    ids.insert(ids.end(), {right_id, left_id});
}

//send its host and port to ringmaster
void Player::send_host_port() {
    char host[MAX_HOST_LEN];
    memset(host, 0, sizeof(host));
    if (gethostname(host, sizeof(host)) != 0) {
        std::cerr << "Error: gethostname() failed\n";
        exit(EXIT_FAILURE);
    }
    send(socket_fd, &host, sizeof(host), 0);

    //work as a server and get port, and send to ringmaster
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t len = sizeof(addr);
    if (getsockname(as_server_fd, (struct sockaddr *)&addr, &len) == -1) {
        std::cerr << "Error: getsockname() failed\n";
        exit(EXIT_FAILURE);
    }
    uint16_t port_num = ntohs(addr.sin_port);
    send(socket_fd, &port_num, sizeof(port_num), 0);
}

// connect with ringmaster and neighbor players
void Player::setup() {
    //connect itself with ringmaster
    socket_fd = create_client(master_port, master_host);
    recv(socket_fd, &id, sizeof(id), MSG_WAITALL);
    recv(socket_fd, &num_players, sizeof(num_players), MSG_WAITALL);

    cal_neighbor_id();

    //work as a server and get port, and send to ringmaster
    as_server_fd = create_server("0");
    send_host_port();

    uint16_t right_port;
    char right_host_cstr[MAX_HOST_LEN];
    memset(right_host_cstr, 0, sizeof(right_host_cstr));
    recv(socket_fd, &right_host_cstr, sizeof(right_host_cstr), MSG_WAITALL);
    recv(socket_fd, &right_port, sizeof(right_port), MSG_WAITALL);

    char right_port_cstr[10];
    memset(right_port_cstr, 0, sizeof(right_port_cstr));
    sprintf(right_port_cstr, "%u", right_port);

    std::cout << "Connected as player " << id << " out of " << num_players << " total players\n";

    // as a client, connect with right
    as_client_fd = create_client(right_port_cstr, right_host_cstr);

    // as a server, connect with left
    //accept
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    client_connect_fd = accept(as_server_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (client_connect_fd == -1) {
        std::cerr << "Error: accept() failed\n";
        exit(EXIT_FAILURE);
    }
    fds.insert(fds.end(), {as_client_fd, client_connect_fd, socket_fd});
}

//receive and send potato
void Player::play_potato() {
    Potato potato;
    srand((unsigned int)time(NULL) + getId());
    while (true) {
        //receive potato from ringmaster or other players
        int n = select_read(fds, potato);
        //if the ringmaster notify that the game ends, jump out of loop
//        if (potato.remain_hops == 0 | n == 0) {
//            break;
//        }
        if (potato.remain_hops == 0) {
            break;
        }
        //if get potato from other player, edit potato
        potato.ids[potato.curr_rnd] = id;
        potato.curr_rnd++;
        potato.remain_hops--;
        if (potato.remain_hops == 0) {
            std::cout << "I'm it\n";
            //send to ringmaster
            send(socket_fd, &potato, sizeof(potato), 0);
            continue;
        }
        //send to a random neighbor
        int random_idx = rand() % 2;
        send(fds[random_idx], &potato, sizeof(potato), 0);
        std::cout << "Sending potato to " << ids[random_idx] << std::endl;
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: program <machine_name> <port_num>\n";
        return EXIT_FAILURE;
    }
    Player player(argv[1], argv[2]);
    player.setup();
    //srand((unsigned int)time(NULL) + player.getId());
    player.play_potato();

    return EXIT_SUCCESS;
}