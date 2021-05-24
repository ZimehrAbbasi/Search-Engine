# Zimehr Abbasi
## CS50 Spring 2021, Lab 6

GitHub username: ZimehrAbbasi

Since this is the last lab of the class, I tried using my own intuition to solve this querier problem, not using any of the hints.
My querier prints the set of documents that contain all the words in the query; you may treat operators ('and', 'or') as regular words.
My querier supports 'and' and 'or' operators, but without precedence (in mathematical terms, it treats them as left associative, equal precedence operators)
My querier supports 'and' precedence over 'or'
My querier program prints the document set in decreasing order by score, thus meeting the full specs.

## ASSUMPTIONS
The primary assumption this querier is that the total number of words in the query will be less than 50.

## LIMITATIONS
This program is limited by the amount of storage space available in the device.
