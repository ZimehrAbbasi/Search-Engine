# Makefile for 'set' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019

PROG = crawler
TEST = testing
OBJS = crawler.o
LIBS = ../libcs50/libcs50.a ../common/common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: crawler.c

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

