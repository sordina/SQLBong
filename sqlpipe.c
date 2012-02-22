#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h> // GetOpt

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

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int main(int argc, char **argv){

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	if( argc == 1 ) {
		fprintf(stderr, "Usage: %s <sql>*\n", argv[0]);
		return(1);
	}

	char c;

	while( ( c = getc(stdin) ) >= 0 ) {
		printf("Char: %c\n", c);
	}

	rc = sqlite3_open(":memory:", &db);

	rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);

	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(db);
	return 0;
}
