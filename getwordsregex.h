#ifndef GETWORDSREGEX
#define GETWORDSREGEX

#include "pcre_split/inc/pcre_split.h"

char** getwordsregex(char* delimiter, char* line, int* numwords) {
	split_t* tokens = pcre_split(delimiter, line);
	split_t* start  = tokens;

#ifdef DEBUG
  pcre_split_print(tokens);
#endif

	*numwords = 0;

	while(start) {
		(*numwords)++;
		start = start->next;
	}

	start  = tokens;

	char** result  = malloc(sizeof(char*) * (*numwords));
	char** resultp = result;

	while(start) {
		*resultp = start->string;
		resultp++;
		start = start->next;
	}

	return result;
}

#endif
