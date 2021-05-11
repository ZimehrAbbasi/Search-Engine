# Makefile for 'set' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019

PROG1 = indexer
PROG2 = indextest
TEST = testing
OBJS1 = indexer.o
OBJS2 = indextest.o
LIBS = ../libcs50/libcs50.a ../common/common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make

$(PROG1): $(OBJS1)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o: indexer.c

$(PROG2): $(OBJS2)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest.o: indextest.c

.PHONY: test valgrind clean

# expects a file `test.names` to exist; it can contain any text.
test: $(TEST).sh
	bash -v $(TEST).sh

# expects a file `test.names` to exist; it can contain any text.
valgrind: $(PROG)
	valgrind ./$(PROG) 

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	rm -rf core

