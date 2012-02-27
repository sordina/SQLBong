
SQLBong
=======


Parse columnized output with SQL - Thanks to the power of SQLite3

The SQL syntax accepted is completely determined by what <a href="http://www.sqlite.org/">SQLite3</a> supports.

Why the name? Because `SQLPipe` was taken.

## Usage:

    > ps | ./sqlbong "select * from data"

  Acts like cat, but normalises the delimiters to one space - Passthrough.

    > cat data.txt | ./sqlbong -f foo.db

  Send columnized data from data.txt into an sqlite database foo.db

## Example:

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
* Data is stored as 'text' type (useful to remember for comparisons, numeric operations)

Known Bugs
----------

* Realloc requires too much memory in getwords.h
* Some dodgey argc mangling going on

To Do
-----

* Add support for command-line arguments
  - Specify delimiter (regex?)
  - Specify alignment for input data if columns are missing
  - Specify how to handle nulls for input, and output
  - Specify to use file database rather than memory (temporary or permanent)
  - Named tables
  - Existing database files, or overwrite
