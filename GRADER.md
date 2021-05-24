# Lab6 rubric

## Student name
Zimehr Abbasi

## Grader name
Tanli Su <tanli.su.22@dartmouth.edu>

Recall the [Assignment](https://github.com/cs50spring2021/tse-labs/tree/main/querier).
​
**Total: 67/100 points**

## (10) Overall

10/10 points

## (8) Makefiles

5/8 points:

* (-2) `querier` builds with warnings when compiled from top-level directory

## (15) Documentation

15/15 points

## (7) Testing

6/7 points:

* (-1) missing `tse/querier/testing.out`

## (20) Coding Style

20/20 points:

* (-3) you should include more spacing in your code (such as after every `if`), and there are places where your code is unnecessarily complicated. See my `GRADING` comment in `querier.c` for an example. You also should not print things to stdout that are not required by the spec, such as "Staring processes..."

## (30) Functionality

5/30 points:

* (-25) your code does not follow the specs - it seems to try to take in a series of queries and then print the results out altogether at the end (whereas the specs require you to parse one query at a time). However, your program fails to even print out the results at the end. From your code it seems like you have some right ideas, but I am unable to test it.

## (10) Memory leaks, according to valgrind

8/10 points:

* (-2) still reachable memory blocks lost