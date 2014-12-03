#ifndef PROCESS_LINEH
#define PROCESS_LINEH

#include <stdio.h>
#include <sqlite3.h>

#include "getwords.h"
#include "build_insert_statement.h"

// Dangerous and stupid, but we're already doing this stringy stuff, so let's do it anyway!
void remove_newlines(char* line) {
	char* p = line;
	while(*p) {
		if(*p == '\n') {
			*p = 0;
			break;
		}
		p++;
	}
}

void process_line(char* line, int* columns, sqlite3* db, globalArgs options) {

	int    return_code;
	char*  zErrMsg;
	int    numwords;
	char   command[400];

	remove_newlines(line);

#ifdef DEBUG
	printf("Inside process_line - Line: %s\n", line);
#endif

	char** words;

	if(options.delimiter) {
		words = getwordsregex(options.delimiter, line, &numwords);
	} else {
		words = getwords(line, &numwords);
	}

#ifdef DEBUG
	if( numwords == 0 || ! words ) { fprintf(stderr, "No words on this line.\n"); }
#endif

#ifdef DEBUG
	int j;
	for(j = 0; j < numwords; j++) {
		printf("Inside process_line - Word: [%s]\n", words[j]);
	}
#endif

	// Adjust table size if needed
	if(numwords > *columns) {

		while(*columns < numwords) {
			(*columns)++;
			sprintf(command, "alter table data add column c%d", *columns);

#ifdef DEBUG
			printf("Inside process_line - Adding new column with command [%s]\n", command);
#endif

			// TODO: Ensure that this is okay
			return_code = sqlite3_exec(db, command, NULL, NULL, &zErrMsg);
			HANDLE_ERROR(return_code);
		}
	}

	if(numwords < 1) {
		return_code = sqlite3_exec(db, "insert into data(c1) values(NULL)", NULL, NULL, &zErrMsg);
		HANDLE_ERROR(return_code);
		return;
	}

	char* insert_statement = insert(numwords);

#ifdef DEBUG
	printf("Inside process_line - Created insert statement [%s].\n", insert_statement);
#endif

	sqlite3_stmt *stmt;

	if ( sqlite3_prepare_v2( db, insert_statement, -1, &stmt, 0 ) != SQLITE_OK ) {
		fprintf(stderr, "Could not prepare statement [%s]\n.", insert_statement);
		exit(1);
	}

#ifdef DEBUG
	printf("Prepared insert statement.\n");
#endif

	int i;
	for(i = 0; i < numwords; i++) {

		// SQLITE_TRANSIENT allows words to be freed
		if ( sqlite3_bind_text( stmt, i+1, words[i], -1 /* length of text */, SQLITE_TRANSIENT ) != SQLITE_OK ) {
			fprintf(stderr,"\nCould not bind text [%s].\n", words[i]);
			exit(1);
		}

#ifdef DEBUG
		printf("Bound word [%s].\n", words[i]);
#endif
// No longer freeing this memory as it seems to cause a segfault
// This should be able to be freed, but I can't identify the real cause for the
// segfault, so taking it out for now.
//
// The fact that this isn't being freed may cause a memory-leak proportional to the
// number of words bound in the query... Which shouldn't be much.
//
// free(words[i]);

	}
	free(words); // words is no longer used

#ifdef DEBUG
	printf("Freed words.\n");
#endif

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		fprintf(stderr,"\nCould not step (execute) stmt.\n");
		exit(1);
	}

	sqlite3_finalize(stmt);
	free(insert_statement);
}

#endif
