
SQLPipe
=======


Parse columnized output with SQL - Thanks to the power of SQLite3

Example:

    > echo "1 2 3 4 5" | ./sqlpipe "select c1, c5, c4 from data" "select c2 from data"

Output:

    1 5 4
    2
