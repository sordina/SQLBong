all:
	gcc sqlbong.c -lsqlite3 -o sqlbong

clean:
	rm sqlbong

test: all
	(echo 111 222 333; echo 444 555 666; echo 777 888 999) | ./sqlbong "select c1, c2 from data"
