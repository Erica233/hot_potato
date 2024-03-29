#ifndef __POTATO_H__
#define __POTATO_H__

#define MAX_HOPS 512

class Potato {
public:
    int tot_hops;
    int remain_hops;
    int curr_rnd;
    int ids[MAX_HOPS] = {};

    Potato(): tot_hops(0), remain_hops(0), curr_rnd(0) {}
    explicit Potato(int n): tot_hops(n), remain_hops(n), curr_rnd(0) {}
    void print_trace() {
        for (int i = 0; i < tot_hops; i++) {
            if (i == tot_hops - 1) {
                std::cout << ids[i] << "\n";
                break;
            }
            std::cout << ids[i] << ",";
        }
    }
};

#endif