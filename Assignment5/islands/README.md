Problem statement
-----------------

While visiting Lakshadweep, you get to know that every island in the place has not just many towns and villages, but also a capital.  Now these are relatively small islands without much infrastructure, so everyone gets around by walking on roads, and everyone in the country walks at exactly the same speed.  Also, people there value their time (they do have many assignments to take care of, after all), so when traveling from one town to another, they always go by the route that will get them there in the least time.  

Since everyone needs to go to the capital once in a while, the capital is defined as the town that, on average, can be reached in the least amount of time from every other town.  If more than one town satisfies this condition, then the town with the first name in alphabetical order is declared the capital of that island.

However, instead of a map, you are just given a list of roads joining towns, and the lengths of these roads.  To make it worse, you don't know how many islands are there either, or which towns are on which island. 

Your job now is to use this information to find all the capitals.

 
Input:
======

For each test case, you are given a list of roads, one per row. The first line has two numbers - the first number is the total number of towns, and the second is the total number of roads.

Each row after the first has two names: one for the starting town, one for the ending town, and a number that gives the distance between the towns. The names of towns are all in lower case letters, with no letters or special characters, and can be up to 20 characters long.  The distance between the towns is an integer (0 < d < 1000) (may be kilometers, but do you really care?). 

Also, unlike the inaptly named Lakshadweep that actually has only 36 islands, 10 of which are inhabited, you may actually find yourself in a place with up to a 1000 islands, each of which may have up to a 1000 towns in it.  

Output:
=======

Print out the names of the capital towns, in alphabetical order.

 

Example
=======

For example, if this were in some other part of the world, you might find the following list of roads -

Input:

9 8
sainteanne fortdefrance 12
bassepointe latrinite 5
bridgetown jackmans 10
fortdefrance ducos 4
holetown bridgetown 12
shanty bridgetown 25
fortdefrance bassepointe 15
jackmans shanty 26

Output:

bridgetown
fortdefrance
 

Bonus question for no marks: the names above are obviously not from Lakshadweep, but are from a real place somewhere on Earth. Where?

TA contact
=======

In case you have any doubts regarding the assignment,
Either post your question on the Moodle class forum, or
Contact Rajat Rao, ee14b101@smail.iitm.ac.in

The first option is strongly recommended.
