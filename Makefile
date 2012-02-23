all:
	# gcc sqlbong.c -lsqlite3 -o sqlbong
	clang -Wall sqlbong.c -lsqlite3 -o sqlbong

clean:
	rm -f sqlbong *.o sql*SYM/

test: all
	# Multi column data
	(echo 111 222 333; echo 444 555 666; echo 777 888 999) | ./sqlbong "select c1, c2 from data"
	# Increasing columns
	(echo 1; echo 1 2; echo 1 2 3; echo 1 2 3 4)           | ./sqlbong "select c1, c2, c4 from data"
	# Decreasing columns
	(echo 1; echo 1 2; echo 1 2 3; echo 1 2; echo 1)       | ./sqlbong "select c1, c2, c4 from data"
