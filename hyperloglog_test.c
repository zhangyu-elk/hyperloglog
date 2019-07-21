#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hyperloglog.h"



int main()
{
	int i = 100;
	hllhdr *hdr = createHLLObject();
	printf("%s\n", "begin------------------");
	
	for(i; i < 199; i++)
	{
		char buff[16] = { 0 };
		uint64_t ret = 0;
		snprintf(buff, 15, "%d", i);
		pfaddCommand(&hdr, buff, 3);
		pfcountCommand(hdr, &ret);
		printf("insert: %d, count: %ld\n", i, ret);
	}

	hdr = hllSparseToDense(hdr);
	assert(hdr != NULL);
	printf("%s\n", "encode change  succ");
	for(i = 100; i < 199; i++)
	{
		char buff[16] = { 0 };
		uint64_t ret = 0;
		snprintf(buff, 15, "%d", i);
		pfaddCommand(&hdr, buff, 3);
		pfcountCommand(hdr, &ret);
		printf("insert: %d, count: %ld\n", i, ret);
	}
	

	for(i = 500; i < 599; i++)
	{
		char buff[16] = { 0 };
		uint64_t ret = 0;
		snprintf(buff, 15, "%d", i);
		pfaddCommand(&hdr, buff, 3);
		pfcountCommand(hdr, &ret);
		printf("insert: %d, count: %ld\n", i, ret);
	}
	sdsfree(hdr);
	printf("%s\n", "end-------------------------------");
	return 0;
}

