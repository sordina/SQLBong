#ifndef GETWORDSH
#define GETWORDSH 1

#include "getword.h"

char** getwords(char* line, int* numwords) { // TODO: Take a size param to make this safe

	char* word;
	int cap = 0;
	char** result = malloc(1);

	*numwords = 0;

	while((word = getword(' ', &line))) {

#if DEBUG
		printf("Inside getwords - Got word: [%s], rest of line: [%s]\n", word, line);
#endif

		if(*numwords >= cap / 2) {
			cap = (cap + 2) * 2 * sizeof(char*); // TODO: Why does this need to be cap + 2 ?
			result = realloc(result, cap);
#ifdef DEBUG
			printf("Inside getwords - Reallocated.\n");
#endif
		}

#ifdef DEBUG
		printf("Inside getwords - Numwords: %d, Cap: %d\n", *numwords, cap);
#endif

		result[*numwords] = word;

#ifdef DEBUG
		printf("Inside getwords - Assigned word to list.\n");
#endif

		(*numwords)++;
	}

	if(0 == *numwords) {
#ifdef DEBUG
		printf("Inside getwords - No words\n");
#endif
		free(result);
		return NULL;
	}

#ifdef DEBUG
	printf("Inside getwords - Finished getting words: %d\n", *numwords);
#endif

	return result;
}

#endif
