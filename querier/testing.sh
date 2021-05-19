# Invalid page directory
./querier invalidir ../data/letters.index
echo " "
# Invalid index file
./querier ../data/letters invalidindex
echo " "
# incorrect number of arguments
./querier arg1 arg2 arg3
echo " "
# Invalid queries
echo "and" | ./querier ../data/letters ../data/letters.index
echo " "
echo "or" | ./querier ../data/letters ../data/letters.index
echo " "
echo "and earth" | ./querier ../data/letters ../data/letters.index
echo " "
echo "planet earth or" | ./querier ../data/letters ../data/letters.index
echo " "
echo "planet earth and" | ./querier ../data/letters ../data/letters.index
echo " "
echo "planet earth and and science" | ./querier ../data/letters ../data/letters.index
echo " "
echo "planet earth or and science" | ./querier ../data/letters ../data/letters.index
echo " "

# Normal cases

echo "first" | ./querier ../data/letters ../data/letters.index
echo " "
echo "first and depth or page" | ./querier ../data/letters ../data/letters.index
echo " "
echo "fourier transform" | ./querier ../data/letters ../data/letters.index
echo " "

echo "reviews" | ./querier ../data/toscrape ../data/toscrape.index
echo " "
echo "reviews and meaning" | ./querier ../data/toscrape ../data/toscrape.index
echo " "
echo "reviews and meaning or baker" | ./querier ../data/toscrape ../data/toscrape.index
echo " "
echo "doctrine" | ./querier ../data/toscrape ../data/toscrape.index
echo " "