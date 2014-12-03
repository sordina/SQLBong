Compiler = clang
Options  = -lm -Wall pcre_split.c sqlbong.c -lpcre -lsqlite3 -o sqlbong

all: usage
	${Compiler} ${Options}

debug: usage
	${Compiler} -ggdb -DDEBUG=1 ${Options}

clean:
	rm -rf sqlbong *.o sql*SYM/

test: all runtests

testdebug: debug runtests

usage:
	# Convert README.md to a printing function
	echo "#ifndef USAGEH"                                                             > usage.h
	echo "#define USAGEH"                                                            >> usage.h
	echo "void usage() {"                                                            >> usage.h
	cat README.md | sed 's/["%\\]/\\&/g;s/^/printf("/;s/$$/\\n");/' | grep -v '<img' >> usage.h
	echo "}"                                                                         >> usage.h
	echo "#endif"                                                                    >> usage.h

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
	(echo ksjdhfklsjdhfksjhdfkjshdl; \
	 echo abdominogenital; \
	 echo skfdjlhasldkfhlaskdjfhlaksdjhflkjahs; \
	 echo foooooooooooofoo fooooof fooooooooooooo)         | ./sqlbong "select c1, c2 from data"
	# Not sure why it's throwing an error, but:
	echo "95585 ttys002    0:03.11 vim sqlbong.c"          | ./sqlbong "select * from data"
	# Lots of words
	echo a b c d e f g h i j k l m n o p q r s t u v w x y | ./sqlbong "select * from data"
	(echo 1_3_2_4; echo 5___6__7_8)                        | ./sqlbong -d '_+' "select c1,c2 from data"
	./sqlbong -d , 'select * from data' < test.csv > /dev/null
	(echo 1 2 3; echo 1 2)                                 | sqlbong -d ' ' 'select * from data' | wc -l | grep 2
