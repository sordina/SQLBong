#include <stdio.h>
#include <sqlite3.h>

#define HANDLE_ERROR(rc)                              \
		if( rc!=SQLITE_OK ){                              \
			fprintf(stderr, "SQL error: %s\n", zErrMsg);    \
			sqlite3_free(zErrMsg);                          \
			exit(1);                                        \
		}

#include "getwords.h"
#include "getwordsregex.h"
#include "build_insert_statement.h"
#include "options.h"
#include "process_line.h"

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
	int columns = 1;
	int i;
	char line[2000]; // TODO: Use a real getline implementation
	int line_buffer_length = 2000;

	if( ! --argc ) {
		fprintf(stderr, "Usage: %s [-h] [-f <output_database>] <sql>*\n", argv[0]);
		exit(1);
	}

	argv++;

	// TODO: Fixme
	globalArgs options = getOpts(argc, argv);

	// Create a database in memory
	if(options.file) {
		rc = sqlite3_open(options.file, &db);
	} else {
		rc = sqlite3_open(":memory:", &db);
	}

	// Create a table with an inital 9 columns. Why 9? Because.
	rc = sqlite3_exec(db, "create table data (c1)", NULL, NULL, &zErrMsg);
	HANDLE_ERROR(rc);

	// Parse stdin, expanding table where necessary
	while(fgets(line, line_buffer_length, stdin)) { process_line(line, &columns, db, options); }

	// Run all queries
	for( i=0 + options.num; i <= argc; i++ ) {

#ifdef DEBUG
		printf("Query: %s\n", argv[i]);
		printf("Results:\n");
#endif

		rc = sqlite3_exec(db, argv[i], callback, NULL, &zErrMsg);
		HANDLE_ERROR(rc);
	}

	sqlite3_close(db);
	return 0;
}
