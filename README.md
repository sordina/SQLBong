
SQLBong
=======


Parse columnized output with SQL - Thanks to the power of <a href="http://www.sqlite.org/">SQLite3</a>.

The SQL syntax accepted is completely determined by what SQLite3 supports.

Why the name? Because `SQLPipe` was taken.

<img src="https://raw.github.com/sordina/SQLBong/master/sqlbong.png" alt="The SQL Bong" />

## Examples:

    > ps | ./sqlbong "select * from data"

  Acts like cat, but normalises the delimiters to one space - Passthrough.

    > cat data.txt | ./sqlbong -f foo.db

  Send columnized data from data.txt into an sqlite database foo.db

    Server> nc -l 1234 | ./sqlbong -f out.db

    Client> nc 127.0.0.1 1234
    hello world
    how   are
    you   ?

    Server> sqlite3 out.db ".dump"
    PRAGMA foreign_keys=OFF;
    BEGIN TRANSACTION;
    CREATE TABLE data (c1, c2);
    INSERT INTO "data" VALUES('hello','world');
    INSERT INTO "data" VALUES('how','are');
    INSERT INTO "data" VALUES('you','?');
    COMMIT;

  Log output to a database on a remote server.

    > cat test.txt | ./sqlbong "select c2 from data limit 2" "select c1, c3, c2 from data where c2 > '4' order by c3 desc"

### test.txt:

    1 2 3
    2 3 4
    3 4 5
    4 5 6
    5 6 7
    6 7 8

### Output:

    2
    3
    6 8 7
    5 7 6
    4 6 5

Details:
--------

* The table used is called 'data'
* There are as many columns in 'data' as the maximum number of columns in the input data
* The columns are labled (c1 .. c\<n\>)
* Data is stored as 'text' type (useful to remember for comparisons, numeric operations)

Known Bugs
----------

* Regex delimitation can cause a segfault on large data-sets

To Do
-----

* Add options for reusing a database

Usage
-----

Accepts data on STDIN, takes SQL statements as arguments (make sure you quote them) and outputs data selected on STDOUT.

Usage:

    sqlbong [-h] [-f <database file>] [-d <regex delimiter>] <sql statement>*

Options:

    -h  Help
    -d  Regex delimiter
    -f  On disk database (defaults to in-memory)

SQL:

    Columns - c1..cn
    Table   - data



