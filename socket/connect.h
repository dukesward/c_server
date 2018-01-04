#ifndef _SERVER_CONNECT_H_
#define _SERVER_CONNECT_H_

#ifndef _SERVER_INCLUDE_H_
  #include "include.h"
#endif

#ifndef _UTILS_H_
  #include "utils.h"
#endif

void socket_create(int port);
void acceptRequest(int sd);
void extractHeader(int sd);
void sendResponse(FILE *f);

#endif

