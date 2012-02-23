#ifndef GETWORDH
#define GETWORDH 1

#include <stdlib.h>
#include <string.h>

char* getword(char delim, char** line) {

	// Skip leading delimiters
	while(**line == delim) {
#ifdef DEBUG
		printf("Inside getword - removing leading delimiter.\n");
#endif
		(*line)++;
	}

	char* word_start = *line;
	char* word_end;

	// Is this an empty line?
	if(**line == EOF || **line == '\n') { return NULL; }

	// Proceed to the end of the word
	while (**line != EOF && **line != '\n' && **line != delim) {
			word_end = *line;
			(*line)++;
	}

	int word_length = 1 + word_end - word_start;

#if DEBUG
	printf("Inside getword - word length: [%d] characters.\n", word_length);
#endif

	if(word_length < 1) {
#ifdef DEBUG
		printf("Inside getword - Blank word.\n");
#endif
		return NULL;
	}

	char* result = malloc(sizeof(char) * (word_length + 1));

	memcpy(result, word_start, word_length);

	result[word_length] = '\0';

	return result;
}

#endif
