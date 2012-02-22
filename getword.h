#ifndef GETWORDH
#define GETWORDH 1

#include <stdlib.h>
#include <string.h>

char* getword(char delim, char** line) { // TODO: Take a size param to make this safe

	char* start = *line;
	char* result;
	int   letters;

	while(**line && **line != '\n' && **line != EOF && **line != delim) { (*line)++; }

	if(start == *line) {
		return NULL;
	}

	letters = (int) (*line - start);

	result = malloc(sizeof(char) * (letters + 1));
	memcpy(result, start, letters);
	result[letters + 1] = '\0';

	while(**line == delim) { (*line)++; }

	printf("Inside getword - Got result:    [%s]\n", result);

	return result;
}

#endif
