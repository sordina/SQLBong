all:
	gcc sqlpipe.c  -lsqlite3 -o sqlpipe

clean:
	rm sqlpipe
