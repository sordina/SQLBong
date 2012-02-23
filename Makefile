Compiler = clang
Options  = -Wall sqlbong.c -lsqlite3 -o sqlbong

all:
	${Compiler} ${Options}

debug:
	${Compiler} -DDEBUG=1 ${Options}

clean:
	rm -f sqlbong *.o sql*SYM/

test: all runtests

testdebug: debug runtests

runtests:
	# Multi column data
	(echo 111 222 333; echo 444 555 666; echo 777 888 999) | ./sqlbong "select c1, c2 from data"
	# Increasing columns
	(echo 1; echo 1 2; echo 1 2 3; echo 1 2 3 4)           | ./sqlbong "select c1, c2, c4 from data"
	# Decreasing columns
	(echo 1; echo 1 2; echo 1 2 3; echo 1 2; echo 1)       | ./sqlbong "select c1, c3, c2 from data"
	# Blank lines
	(echo 1 3 2 4; echo; echo 1 2 3)                       | ./sqlbong "select c2 from data"
	# Long words
	(echo ksjdhfklsjdhfksjhdfkjshdl; echo abdominogenital) | ./sqlbong "select c1 from data"
