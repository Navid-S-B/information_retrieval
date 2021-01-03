// Navid Bhuiyan (z5260384)
// avltwordcountree ADT interface
#ifndef AVLTREEWORDCOUNT_H
#define AVLTREEWORDCOUNT_H
#include "avltreewordcount.h"
#define WORD_LENGTH 101

// Custom Node Types
typedef struct llnode *LLNode;
struct llnode {
    char *key;
    LLNode next;
};

typedef struct ll *LL;
struct ll {
    LLNode head;
    LLNode tail;
};

typedef struct avlnode *AvlNode;
struct avlnode {
    char *word;
    int word_count;
    int  height;
    AvlNode left;
    AvlNode right;
};

typedef struct avltree *Avl;
struct avltree {
    AvlNode root;
    int total_word_count;
    LL keys;
};

// Redefinition of string.h function (not a standard function apparently)?
char *strdup(const char *str);

// Make new linked list
LL LinkedListNew(void);

// Insert into LinkedList
void LinkedListInsert(LL ll, char *key);

// Creates a new empty tree
Avl TreeNew(void);

// Frees all memory associated with the given tree
void TreeFree(Avl t);

// Inserts value into AVL
void TreeInsert(Avl t, char *word);

// Searches tree with known key.
// (Meant to be used with the tree's keys)
AvlNode TreeSearch(Avl t, char *key);

// List the tree
void TreeList(Avl t);

// Shows the tree structure
void TreeShow(Avl t);

#endif
