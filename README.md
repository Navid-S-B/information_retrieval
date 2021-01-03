# Information Retrieval: Word Indexing
This project utilises abstract data structures (AVL Tree, Binary Search Tree and Linked Lists) <br />
to examine the wording information in certain text files. The words are indexed using a method <br />
called "tf-idf", which considers the word frequency within a file. <br />

## How to Examine the Code
There are test text files in the testFiles folder. Right now there are preset tests mentioned <br />
in the test_invertIndex shell script which ensure correctness in simple cases <br />.

To run the test simply run this command on a Bash supported machine: <br />
#### `sh test_invertedIndex.sh`

If you want to check the index information of other files, please customise the test_invertedIndex.c file <br />
and simply compile all of the files with the text file using <br />
#### `gcc -Wall -Werror -lm -std=c11 *.c -o invertedIndex`

## Other Notes
This project recieved a mark of 96%.<br />
