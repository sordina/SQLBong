all:
	gcc sqlpipe.c -lsqlite3 -o sqlpipe

clean:
	rm sqlpipe

test: all
	(echo 1 2 3; echo 4 5 6; echo 7 8 9) | ./sqlpipe "select c1, c2 from data"
