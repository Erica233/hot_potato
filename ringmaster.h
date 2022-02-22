#ifndef __RINGMASTER_H__
#define __RINGMASTER_H__

#define MAX_NUM_HOPS 512

class Ringmaster {
private:
    char * port;
    int num_players;
    int num_hops;
public:
    Ringmaster(char * port_num, char * n_players, char * n_hops): {
        port = port_num;
        num_players = atoi(n_players);
        if (num_players <= 1) {
            std::cerr << "Invalid num_players\n";
            return EXIT_FAILURE;
        }
        int num_hops = atoi(argv[3]);
        if (num_hops < 0 || num_hops > MAX_NUM_HOPS) {
            std::cerr << "Invalid num_hops\n";
            return EXIT_FAILURE;
        }
    }
};

#endif
