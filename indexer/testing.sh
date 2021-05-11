./indexer
./indexer ./indexer 
./indexer ./indexer ../data/invaliddir ../data/out ../data/invaliddir ../data/out
./indexer ../data/invaliddir ../data/out
./indexer ../data/letters ../data/unreadable.txt
./indexer ../data/letters ../data/letters.index

./indexer ../data/toscrape ../data/toscrape.index
./indexer ../data/wikipedia ../data/wikipedia.index

./indextest ../data/letters.index  ../data/lettersnew.index
./indextest ../data/toscrape.index  ../data/toscrapenew.index
./indextest ../data/wikipedia.index  ../data/wikipedianew.index