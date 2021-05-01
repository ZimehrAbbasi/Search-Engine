# Makefile for 'set' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019

PROG = crawler
TEST = testing
OBJS = crawler.o ../libcs50/hashtable.o ../libcs50/bag.o ../libcs50/webpage.o ../libcs50/set.o ../libcs50/mem.o ../libcs50/hash.o ../libcs50/file.o ../common/pagedir.o
LIBS =

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: crawler.c
../libcs50/hashtable.o: ../libcs50/hashtable.h
../libcs50/bag.o: ../libcs50/bag.h
../libcs50/webpage.o: ../libcs50/webpage.h
../libcs50/set.o: ../libcs50/set.h
../libcs50/mem.o: ../libcs50/mem.h
../libcs50/hash.o: ../libcs50/hash.h
../libcs50/file.o: ../libcs50/file.h
../common/pagedir.o: ../common/pagedir.h

.PHONY: test valgrind clean

# expects a file `test.names` to exist; it can contain any text.
test: $(TEST).sh
	chmod +x $(TEST).sh
	./$(TEST).sh

# expects a file `test.names` to exist; it can contain any text.
valgrind: $(PROG)
	valgrind ./$(PROG)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	rm -rf core

