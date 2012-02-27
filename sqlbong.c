#include <stdio.h>
#include <sqlite3.h>

#define HANDLE_ERROR(rc)                              \
		if( rc!=SQLITE_OK ){                              \
			fprintf(stderr, "SQL error: %s\n", zErrMsg);    \
			sqlite3_free(zErrMsg);                          \
			exit(1);                                        \
		}

#include "getwords.h"
#include "build_insert_statement.h"
#include "process_line.h"
#include "options.h"

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
	int columns = 1;
	int head = 0;
	char line[10000]; // TODO: Use a real getline implementation
	int line_buffer_length = 1000;

	if( ! --argc ) {
		fprintf(stderr, "Usage: %s <sql>*\n", argv[0]);
		return(1);
	}

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
	while(fgets(line, line_buffer_length, stdin)) { process_line(line, &columns, db); }

	// Run all queries
	for( head=1 + options.num * 2; head <= argc; head++ ) {

#ifdef DEBUG
		printf("Query: %s\n", argv[head]);
		printf("Results:\n");
#endif

		rc = sqlite3_exec(db, argv[head], callback, NULL, &zErrMsg);
		HANDLE_ERROR(rc);
	}

	sqlite3_close(db);
	return 0;
}
