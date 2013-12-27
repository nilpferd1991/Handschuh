#include "rsconnect_tools.h"

void tools::wait(unsigned long nanosec)
{
	struct timespec req = {0};
	req.tv_sec = 0;
	req.tv_nsec = nanosec;
	while (nanosleep(&req, &req) == -1)
		continue;
	return;
}
