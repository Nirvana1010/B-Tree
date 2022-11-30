# B-Tree

**Platform:** 

Visual Studio 2010.

**Requirements:**

a) Refer to the sample code in the book to indicate the keyword for each record as an integer. For simplicity, each record can contain only one keyword (i.e., the other fields can be undefined).

b) Starting from the empty tree, enter each keyword in turn to build the corresponding B-tree.

c) And implement the insertion and deletion of keywords in the B-tree.

d) Display the tree shape on the screen at the same time. The tree shape is displayed when all keywords are inserted, and the current tree shape is also displayed after each insertion or deletion operation is completed.

e) Assume that there are no duplicate keywords in the B-tree.

f) When the program is running, the input format must be given on the screen when data is to be entered. When there is output displayed, the necessary explanation of the output is also required.

**Input data format:**
  The input format when creating a B-tree is to read the following data from the keyboard.

10 3

20, 25, 4, 50...

The two numbers in the first line indicate the number of keywords and the order of the B-tree to be built, separated by spaces or special symbols of your own definition (preferably with a message line). In this example, there are 10 keywords, and the B-tree is of order 3.

The second line contains all the keywords, separated by commas, and can be followed by the next line if it does not fit in one line. When enough keywords are read, the subsequent integers are ignored.
  To insert or delete a node, read the following data from the keyboard.

I 76

D 50

The first and second lines represent examples of node insertion and deletion, respectively. The operation can be repeated, i.e. inserting nodes consecutively or deleting nodes consecutively. The following format can also be used.

MI 76, 23, 51#

MD 50, 4, 14#

These two lines indicate the insertion or deletion of multiple keywords, separated by commas and terminated by a #. When a duplicate keyword appears, a message is given to indicate "duplicate keyword" and the duplicate input is ignored; when a non-existent keyword is deleted, a message is also given to indicate "corresponding keyword not found".

**Output data format:**

Output tree in your own defined form.
