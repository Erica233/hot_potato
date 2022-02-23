#include "player.h"

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

void Player::send_host_port() {
    char host[MAX_HOST_LEN];
    memset(host, 0, sizeof(host));
    if (gethostname(host, sizeof(host)) != 0) {
        std::cerr << "Error: gethostname() failed\n";
        exit(EXIT_FAILURE);
    }
    //std::cout << "----my host from gethostname(): " << host << std::endl;
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
    //std::cout << "----my port_num: " << port_num << std::endl;
    send(socket_fd, &port_num, sizeof(port_num), 0);
}

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
    //std::string right_host(right_host_cstr);
    //std::cout << "right_id: " << right_id << std::endl;
    //std::cout << "right_port: " << right_port << std::endl;
    //std::cout << "right_host: " << right_host << std::endl;
    //std::cout << "right_host_cstr: " << right_host_cstr << std::endl;
    char right_port_cstr[10];
    memset(right_port_cstr, 0, sizeof(right_port_cstr));
    sprintf(right_port_cstr, "%u", right_port);
    //std::cout << "right_port_cstr: " << right_port_cstr << std::endl;

    //std::cout << "Connected as player " << id << " out of " << num_players << " total players\n";

    // as a client, connect with right
    //std::cout << "as a client, connect with right: " << std::endl;
    as_client_fd = create_client(right_port_cstr, right_host_cstr);

    // as a server, connect with left
    //std::cout << "as a server, connect with left: " << std::endl;
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

void Player::play_potato() {
    //play
    Potato potato;
    //std::cout << "=======before potato: \n";
    while (true) {
        //std::cout << "------enter while\n";
        //receive potato from ringmaster or other players
        int n = select_read(fds, potato);
        //potato.print_trace();
        //std::cout << "after select_read()\n";
        //std::cout << "curr_rnd: " << potato.curr_rnd << std::endl;
        //if the ringmaster notify that the game ends, jump out of loop
        if (potato.remain_hops == 0 | n == 0) {
            //std::cout << "game ends\n";
            break;
        }
        //if get potato from other player, edit potato
        //std::cout << "edit potato: \n";
        potato.ids[potato.curr_rnd] = id;
        //std::cout << "ids[potato.curr_rnd] = " << potato.ids[potato.curr_rnd] << std::endl;
        potato.curr_rnd++;
        //std::cout << "curr_rnd: " << potato.curr_rnd << std::endl;
        potato.remain_hops--;
        //std::cout << "potato.remain_hops: " << potato.remain_hops << std::endl;
        if (potato.remain_hops == 0) {
            std::cout << "I’m it\n";
            //potato.print_trace();
            //send to ringmaster
            send(socket_fd, &potato, sizeof(potato), 0);
            continue;
        }
        //send to a random neighbor
        int random_idx = rand() % 2;
        //std::cout << "to random_idx: " << random_idx << std::endl;
        send(fds[random_idx], &potato, sizeof(potato), 0);
        //potato.print_trace();
        std::cout << "Sending potato to " << ids[random_idx] << std::endl;
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: program <machine_name> <port_num>\n";
        return EXIT_FAILURE;
    }
    Player player(argv[1], argv[2]);
    //std::cout << "master_host = " << player.master_host << std::endl;
    //std::cout << "master_port = " << player.master_port << std::endl;

    player.setup();
    //std::cout << "num_players = " << player.num_players << std::endl;
    //std::cout << "left_id = " << player.left_id << std::endl;
    //std::cout << "id = " << player.id << std::endl;
    //std::cout << "right_id = " << player.right_id << std::endl;

    srand((unsigned int)time(NULL) + player.id);
    player.play_potato();

    return EXIT_SUCCESS;
}