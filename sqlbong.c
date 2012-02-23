#include <stdio.h>
#include <sqlite3.h>

// #define DEBUG 1

#include "getwords.h"
#include "question.h"

/*
 * Example: (echo "1 2 3"; echo "4 5 6"; echo "7 8 9") | sqlbong "select c1 c3 from data" "select c2 + c3 from data where c1 + c2 > 5 order by c3 desc"
 *
 * Output:  1  3
 *          4  6
 *          7  9
 *          17
 *          11
 */

const char sep_default = ' ';

static int callback(void* userArg, int count, char** columns, char** column_names){

#ifdef DEBUG
	printf("Columns in query: %d\n", count);
#endif

	int i;
	for(i=0; i<count; i++){

		// Main Program output!
		printf("%s", columns[i] ? columns[i] : " "); // TODO: Decide what to do with null columns

		if(i + 1 < count) {
			printf(" "); // TODO: Space columns more nicely somehow
		}
	}
	printf("\n");
	return 0;
}

int main(int argc, char **argv){

#ifdef DEBUG
	printf("Inside main.\n");
#endif

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char** words;
	int numwords;
	int columns = 1;
	int head = 0;
	char line[10000]; // TODO: Use a real getline implementation
	int line_buffer_length = 1000;

	if( ! --argc ) {
		fprintf(stderr, "Usage: %s <sql>*\n", argv[0]);
		return(1);
	}

	// Create a database in memory
	rc = sqlite3_open(":memory:", &db);

	// Create a table with an inital 9 columns. Why 9? Because.
	rc = sqlite3_exec(db, "create table data (c1)", NULL, NULL, &zErrMsg);
	if( rc!=SQLITE_OK ){

#ifdef DEBUG
		fprintf(stderr, "SQL error: [%s], For table creation.\n", zErrMsg);
#endif

		sqlite3_free(zErrMsg);
		return(1);
	}

	// Parse stdin, expanding table where necessary
	while(fgets(line, line_buffer_length, stdin)) {

#ifdef DEBUG
		printf("Line: %s\n", line);
#endif

		words = getwords(line, &numwords);

#ifdef DEBUG
		if( ! words ) {
			fprintf(stderr, "No words on this line.\n");
		}
#endif

#ifdef DEBUG
		int j;
		for(j = 0; j < numwords; j++) {
			printf("Inside SQLBong - Word: [%s]\n", words[j]);
		}
#endif

		// Adjust table size if needed
		if(numwords > columns) {

			char command[400]; // TODO: Possible overrun here

			while(columns < numwords) {
				columns++;
				sprintf(command, "alter table data add column c%d", columns);

#ifdef DEBUG
				printf("Adding new column with command [%s]\n", command);
#endif

				rc = sqlite3_exec(db, command, NULL, NULL, &zErrMsg);
				if( rc!=SQLITE_OK ){
					fprintf(stderr, "SQL error: [%s], For command [%s]\n", zErrMsg, command);
					sqlite3_free(zErrMsg);
					return(1);
				}
			}
		}

		char* insert_statement = insert(numwords);

#ifdef DEBUG
		printf("Created insert statement [%s].\n", insert_statement);
#endif

		sqlite3_stmt *stmt;

		if ( sqlite3_prepare( db, insert_statement, -1, &stmt, 0 ) != SQLITE_OK ) {
			fprintf(stderr, "Could not prepare statement [%s]\n.", insert_statement);
			return 1;
		}

		int i;
		for(i = 0; i < numwords; i++) {
			if ( sqlite3_bind_text ( stmt, i+1, words[i], -1 /* length of text */, SQLITE_TRANSIENT ) != SQLITE_OK ) {
				fprintf(stderr,"\nCould not bind int to word [%s].\n", words[i]);
				return 1;
			}

			free(words[i]); // SQLITE_TRANSIENT allows words to be freed
		}

		free(words); // words is no longer used

		if (sqlite3_step(stmt) != SQLITE_DONE) {
			fprintf(stderr,"\nCould not step (execute) stmt.\n");
			return 1;
		}
	}

	// Run all queries
	for( head=1; head <= argc; head++ ) {

#ifdef DEBUG
		printf("Query: %s\n", argv[head]);
		printf("Results:\n");
#endif

		rc = sqlite3_exec(db, argv[head], callback, NULL, &zErrMsg);

		if( rc!=SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
			return(1);
		}
	}

	sqlite3_close(db);
	return 0;
}
