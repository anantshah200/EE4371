Summary: 
--------
Find all words from a given set that are anagrams of each other.

Example:
--------
Given a set of words (between the --- lines)
---
acts
ball
cats
dog
elephant
fish
god
hand
scat
cast
---
the program should print out information about all the anagrams that were found.  For example, 'acts' , 'cats', 'scat' are anagrams, because they consist of the same letters in different orders.

Input format:
-------------
First line of input contains number of words (N) in the set.
Next N lines have one word per line, where each word consists of the letters 'a'-'z' (may be capital), numbers and the apostrophe (') symbol.
When comparing anagrams, ignore whether a letter was capital or lower case.

Expected output:
----------------
Line 1: number of anagram sets found
Line 2: number of anagrams in first set (K)
Line 3 to K+2: list of anagrams in first set, one word per line, in alphabetical order.
<Repeat for next set etc. till all anagrams printed out>

Example output (for above input):
---------------------------------
---
2
4
acts
cast
cats
scat
2
dog
god
---
