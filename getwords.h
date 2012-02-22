#ifndef GETWORDSH
#define GETWORDSH 1

#include "getword.h"

char** getwords(char* line, int* numwords) { // TODO: Take a size param to make this safe

	char* p = line;
	char* word;
	int cap = 0;
	char** result = malloc(1);

	*numwords = 0;

	while(word = getword(' ', &p)) {

		printf("Inside getwords - Got word:     [%s]\n", word);
		printf("Inside getwords - Rest of line: [%s]\n", p);

		if(*numwords >= cap) {
			cap = (cap + 1) * 2 * sizeof(char*);
			realloc(result, cap);
			printf("Inside getwords - Reallocated.\n");
		}

		printf("Inside getwords - Numwords: %d, Cap: %d\n", *numwords, cap);

		result[*numwords] = word;

		printf("Inside getwords - Assigned word to list.\n");

		(*numwords)++;
	}

	printf("Inside getwords - Finished getting words: %d\n", *numwords);

	return result;
}

void freewords(char** words, int num) {
	char** p = words;

	while( --num ) {
		free(*(p++));
	}

	free(words);
}

#endif
