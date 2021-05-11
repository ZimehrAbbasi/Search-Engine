# Makefile for 'set' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019

OBJS = index.o pagedir.o word.o ../libcs50/bag.o ../libcs50/counters.o ../libcs50/file.o ../libcs50/hashtable.o ../libcs50/hash.o ../libcs50/mem.o ../libcs50/set.o ../libcs50/webpage.o 
LIB = common.a 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

# Build $(LIB) by archiving object files
# (you will need to drop in copy of set.c, counters.c, hashtable.c)
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

index.o: index.h
pagedir.o: pagedir.h
word.o: word.h
bag.o: ../libcs50/bag.h
counters.o: ../libcs50/counters.h
file.o: ../libcs50/file.h
hashtable.o: ../libcs50/hashtable.h ../libcs50/set.h ../libcs50/hash.h
hash.o: ../libcs50/hash.h
mem.o: ../libcs50/mem.h
set.o: ../libcs50/set.h
webpage.o: ../libcs50/webpage.h

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
# (this rule is used only by the Professor in preparing the starter kit)
sourcelist: Makefile *.md *.c *.h
	@echo $^

clean:
	rm -f core
	rm -f $(LIB) *~ *.o
