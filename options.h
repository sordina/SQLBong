
#include <stdio.h>
#include <string.h>

typedef struct globalArgs_t {
	char* file;
	char* delimiter;
} globalArgs;

globalArgs getOpts(int* argc, char** argv) {

	int i;
	int max = 0;

	globalArgs result;

	result.file      = NULL;
	result.delimiter = NULL;

	for(i = 0; i < (*argc) - 1; i++) {
		if(       strcmp("-f",argv[i])) {
			result.file = argv[i+1];
			max = i;

		} else if(strcmp("-d",argv[i])) {
			result.delimiter = argv[i+1];
			max = i;
		}
		i += 2;
	}

	return result;
}
