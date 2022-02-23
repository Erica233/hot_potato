#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <vector>

#include "potato.h"

#define BACK_LOG 100
#define MAX_HOST_LEN 255

int create_server(const char * port);
int create_client(const char * port, const char * hostname);
void select_read(std::vector<int> & fds, Potato & potato);

#endif
