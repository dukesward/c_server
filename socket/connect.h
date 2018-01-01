#ifndef _SERVER_CONNECT_H_
#define _SERVER_CONNECT_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netinet/tcp.h>

#include <arpa/inet.h>

#include "nonblock.h"

void socket_create(int port);
void acceptRequest(int sd);
void extractHeader(int sd);
void sendResponse(FILE *f);

#endif

