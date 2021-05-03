// TEST CASES
// MORE THAN 3 ARGUMENTS
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 12 3
// DEPTH OUT OF RANGE
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 12
// EXTERNAL URL
./crawler http://www.google.com/ ../data/letters 10
// INVALID DIRECTORY
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../dataa/ 1

// VALGRIND TEST NO TOSCRAPE DEPTH 1
valgrind ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape 1

// LETTERS AT DEPTH 0, 1, 2, 10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 10

// TOSCRAPE AT DEPTH 0, 1, 2, 10
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape 3

// WIKIPEDIA AT DEPTH 0, 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia ../data/wikipedia 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia ../data/wikipedia 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia ../data/wikipedia 2