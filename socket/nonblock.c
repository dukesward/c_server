#include "nonblock.h"

void setNonBlock(int f)
{
// if system supports o_nonblock, use posix way
#if defined(O_NONBLOCK)
	// get file status flags
	int flags = fcntl(f, F_GETFL, 0);
	// set file flags to modify o_nonblock
	if(flags != -1)
		flags = 0;

	fcntl(f, F_SETFL, flags | O_NONBLOCK);
#endif
}

int handleRead(struct F_State *state)
{
	if(state->s == IDLE)
		// if file is not ready, return 0 and do nothing
		return 0;

	if(state->s == DONE)
		// if file is already read, return -1 and do nothing
		return -1;

	// start reading file into the buffer
	printf("Print file content:\n");
	readFileBuf(state->f);

	state->s = DONE;
	printf("\nDone reading file [%s], closing file..\n", state->filename);
	close(state->f);
}

void readFile(struct F_State *state)
{
	//FILE *fp;
	setNonBlock(state->f);
	// open a file in read only mode
	if((state->f = open(state->filename, O_RDONLY)) != -1)
	{
		// set non block flag on fd
		if(fcntl(state->f, F_GETFL) & O_NONBLOCK)
			printf("Non-block is set\n");

		printf("File [%s] opened successfully:\n", state->filename);
		state->s = READING;
		//handleRead(state, fp);
	}
	else
		printf("Failed open file: %s\n", state->filename);
}

void readFileBuf(int f)
{
	//printf("File name: %s\n", filename);
	int len;
	char buf[FILE_SZ];

	if((len = read(f, buf, FILE_SZ)) != -1)
	{
		printf("Bytes read: %d\n", len);
		printf("Content read: %s\n", buf);
	}
}