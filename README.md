
SQLPipe
=======


Parse columnized output with SQL - Thanks to the power of SQLite3

## Example:

    > cat test.txt | ./sqlpipe "select c2 from data limit 2" "select c1, c3, c2 from data where c2 > '4' order by c3 desc"

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

* Does not currently handle all input
* Realloc requires too much memory in getwords.h
