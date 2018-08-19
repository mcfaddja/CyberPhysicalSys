#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>


int main(int argc, char *argv[])
{
	char buff[256];
	uint64_t trackuid = 2906622092;
	
	sprintf(buff, "%" PRIu64, trackuid);

	printf("uid is %s", buff);
}
