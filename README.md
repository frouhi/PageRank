## Search Engine Implementation Using PageRank Algorithm ##

**Author**
* Farhang Rouhi

**Introduction**

This program is a search engine implementation that uses the PageRank algorithm. The program receives a graph as an argument.
Each node is a path to a file. The program uses the PageRank algorithm to determine the rank of each file.
Next, it reads in all of these text files and then parses them into tokens and stores those tokens in a hash table.
This hash table is implemented using arrays and linked lists.
At this point, the program searches for the given token and prints out the highest ranked five pages corresponding to that token.

**How to use**

To compile, use the make command. The Makefile is included.

Name of the executable is query. Use **./query graph_name keyword**. This will search the files in the graph for the keyword.
One set of files and a graph is included in data directory. To use this example run **./query ../data/graph.txt keyword**.

**Components**

* **graphUtils.c:** This component gets the graph as an input, runs the PageRank algorithm, and outputs the rank of each file.
* **hashtable.c:** This component is a hashtable implementation using arrays and linked lists.
* **indexFiles.c:** This component reads in text files and then parses them into tokens and stores those tokens in a hash table (implemented in hashtable.c).
* **query.c:** This component uses other components to:
     1. read in a graph.
     2. calculate page ranks.
     3. Parse the corresponding text files to the nodes in the graph into tokens.
     4. Store these tokens in a hash table.
     5. Provide functionality of searching for a keyword or token.
