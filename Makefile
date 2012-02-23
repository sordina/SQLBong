all:
	gcc sqlpipe.c -lsqlite3 -o sqlpipe

clean:
	rm sqlpipe

test: all
	(echo 111 222 333; echo 444 555 666; echo 777 888 999) | ./sqlpipe "select c1, c2 from data"
