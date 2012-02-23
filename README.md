
SQLPipe
=======


Parse columnized output with SQL - Thanks to the power of SQLite3

Example:

    > echo "1 2 3 4 5" | ./sqlpipe "select c1, c5, c4 from data" "select c2 from data"

Output:

    1 5 4
    2

Example:

    > cat test.txt | ./sqlpipe "select c1, c3, c2 from data where c2 > '4' order by c3 desc"

Data - test.txt:

    1 2 3
    2 3 4
    3 4 5
    4 5 6
    5 6 7
    6 7 8

Output:

    6 8 7
    5 7 6
    4 6 5


Known Bugs
----------

* Does not currently handle all input
* Realloc requires too much memory in getwords.h
