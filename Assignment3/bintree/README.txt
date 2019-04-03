Summary: 
--------
Given a set of numbers (assume they are all distinct positive integers), 
first create a binary search tree by inserting the numbers
in the order in which they are read, and then print out the different traversals
of the tree.  Note that the in-order traversal will give the numbers 
in sorted order.  However, the pre- and post-order traversals will give the 
numbers in some other order that depends on the order in which they were read.

Example:
--------
Given a list of numbers
---
23
41
5678
981203
8
89
21
788
9012
152
---

Input format:
-------------
First line of input contains number of numbers (N) in the list
Next N lines have one positive integer per line.

Expected output:
----------------
Line 1-N: all the input numbers in in-order form (sorted)
Line N+1-2N: all numbers in pre-order form
Line 2N+1-3N: all numbers in post-order form

Example output (for above input):
---------------------------------
---
8
21
23
41
89
152
788
5678
9012
981203
23 
8 
21 
41 
5678 
89 
788 
152 
981203 
9012 
21 
8 
152 
788 
89 
9012 
981203 
5678 
41 
23
---
