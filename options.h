#ifndef OPTIONSH
#define OPTIONSH

#include <stdio.h>
#include <string.h>
#include "usage.h"

typedef struct globalArgs_t {
	char* file;
	int num;
	// int overwrite;    // TODO: Useful option
	// char* delimiter;  // TODO: Useful option
	// char* table_name; // TODO: Useful option
} globalArgs;

globalArgs getOpts(int argc, char** argv) {

#ifdef DEBUG
	printf("Inside getOpts - Argc [%d].\n", argc);
#endif

	int i;
	globalArgs result;

	result.file      = NULL;
	result.num       = 0;
	// result.table_name = NULL
	// result.overwrite  = 1;
	// result.delimiter  = NULL;

	// NOTE: Incrementing i by 2 as all flags take a param
	for(i = 0; i < argc; i++) {
#ifdef DEBUG
		printf("Inside getOpts - Arg %d - [%s].\n", i, argv[i]);
#endif
		if(0 == strcmp("-f", argv[i])) {
			result.file = argv[i+1]; // TODO: Why is this +2?
			result.num += 2;
			i++;
		} else
		if(0 == strcmp("-h",argv[i])) {
			usage();
			exit(1);
		}
	}

	return result;
}

#endif
