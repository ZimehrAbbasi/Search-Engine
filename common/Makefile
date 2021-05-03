# Makefile for 'set' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019

OBJS = pagedir.o 
LIBS = common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h

.PHONY: clean

clean:
	rm -f core
	rm -f $(LIB) *~ *.o
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
