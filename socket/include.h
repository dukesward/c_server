#ifndef _SERVER_INCLUDE_H_
#define _SERVER_INCLUDE_H_

// system specific headers
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
// network related headers
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
// std libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// common libraries
#include <string.h>
#include <fcntl.h>
#include <errno.h>

extern const char* _CONFIG_;

enum STATE
{
	IDLE,
	READING,
	DONE
}; 

struct F_Config {
	char *key;
	char *value;
	int index;
	struct F_Config *next;
};

struct F_State
{
	const char *filename;
	int f;
	enum STATE s;
	struct F_Config *first_ln;
};

struct Dictionary {
	int items;
	struct F_Config *c;
};

#endif