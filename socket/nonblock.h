#ifndef _NONBLOCK_H_
#define _NONBLOCK_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define FILE_SZ 256

enum STATE
{ 
	IDLE,
	READING,
	DONE
};
// for reading multiple files, must use a struct
struct F_State
{
	char *filename;
	int f;
	enum STATE s;
};

void setNonBlock(int f);
int handleRead(struct F_State *state);
void readFile(struct F_State *state);
void readFileBuf(int f);

#endif