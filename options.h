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

	int i;
	globalArgs result;

	result.file      = NULL;
	result.num       = 0;
	// result.table_name = NULL
	// result.overwrite  = 1;
	// result.delimiter  = NULL;
	
#ifdef DEBUG
	printf("Inside Options - Printing arguments:\n");
	for(i = 0; i < argc; i++ ) {
		printf("Arg %d - [%s].\n", i, argv[i]);
	}
#endif

	// NOTE: Incrementing i by 2 as all flags take a param
	for(i = 0; i < argc - 1; i += 2 ) {
		if(strcmp("-f",argv[i])) {
			result.file = argv[i+2]; // TODO: Why is this +2?
			result.num++;

		} else if(strcmp("-h",argv[i])) {
			usage();
			exit(1);
		// } else if(strcmp("-d",argv[i])) {
		//	result.delimiter = argv[i+2];
		//	result.num++;
		}
	}

	return result;
}

#endif
