#include <stdio.h>
#include <sqlite3.h>
#include "getwords.h"

/*
 * Example: (echo "1 2 3"; echo "4 5 6"; echo "7 8 9") | sqlpipe "select c1 c3 from data" "select c2 + c3 from data where c1 + c2 > 5 order by c3 desc"
 *
 * Output:  1  3
 *          4  6
 *          7  9
 *          17
 *          11
 */

const char sep_default = ' ';

static int callback(void *userArg, int count, char **columns, char **column_names){
	printf("Columns in query: %d\n", count);
	int i;
	for(i=0; i<count; i++){
		printf("%s", columns[i] ? columns[i] : "\"\"");
		if(i + 1 < count) {
			printf(" "); // TODO: Space columns more nicely somehow
		}
	}
	printf("\n");
	return 0;
}

int main(int argc, char **argv){

	printf("Inside main.\n");

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char c;
	char** words;
	int numwords;
	int columns = 1;
	int head = 0;
	char line[10000]; // TODO: Use a real getline implementation

	if( ! --argc ) {
		fprintf(stderr, "Usage: %s <sql>*\n", argv[0]);
		return(1);
	}

	// Create a database in memory
	rc = sqlite3_open(":memory:", &db);

	// Create a table with an inital 9 columns. Why 9? Because.
	rc = sqlite3_exec(db, "create table data (c1)", NULL, NULL, &zErrMsg);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: [%s], For table creation.\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return(1);
	}

	// Parse stdin, expanding table where necessary
	while(gets(line)) {
		printf("Line: %s\n", line);
		words = getwords(line, &numwords);
		if( words ) {
			int j;
			for(j = 0; j < numwords; j++) {
				printf("Inside SQLPipe - Word: [%s]\n", words[j]);
			}

			// Adjust table size if needed
			if(numwords > columns) {
				char command[400];
				columns = numwords;
				sprintf(command, "alter table data add column c%d", columns);
				rc = sqlite3_exec(db, command, NULL, NULL, &zErrMsg);
				if( rc!=SQLITE_OK ){
					fprintf(stderr, "SQL error: [%s], For command [%s]\n", zErrMsg, command);
					sqlite3_free(zErrMsg);
					return(1);
				}
			}

			// Insert Data
			char command[400];
			sprintf(command, "alter table data add column c%d", columns);
			rc = sqlite3_exec(db, command, NULL, NULL, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: [%s], For command [%s]\n", zErrMsg, command);
				sqlite3_free(zErrMsg);
				return(1);
			}

			freewords(words, numwords);
		} else {
			fprintf(stderr, "Error accessing getword results.\n");
		}
	}

	// Run all queries
	for( head=1; head <= argc; head++ ) {
		printf("Query: %s\n", argv[head]);
		printf("Results:\n");
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
