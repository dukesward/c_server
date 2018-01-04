#include "utils.h"

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