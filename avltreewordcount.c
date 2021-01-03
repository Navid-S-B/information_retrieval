// Navid Bhuiyan (z5260384)
// Assignment 1 Custom ADT for word count storage.
/*
There was either the implementation of a hash table or a BST structure,
however as the hash table is powerful, it was too complicated for me 
to implement it in such a short span. So I decided to use a AVL  as its
BST is always balanced and has O(logn) look up efficiencies to substitute 
a hash table.

It had used Lab05 AVL Avl Implementation from COMP2521 Term 3 2020 for reference.

It creates a binary tree where it keeps the keys, their word counts (at root) and total word
count at individual nodes.
*/ 

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avltreewordcount.h"

////////////////////////////////////////////////////////////////////////
// Function Prototypes

static void doFree(AvlNode n);
static AvlNode doInsert(Avl t, AvlNode n, char *word);
static AvlNode rotateLeft(AvlNode n);
static AvlNode rotateRight(AvlNode n);
static int  height(AvlNode n);
static int  max(int a, int b);
static AvlNode newNode(char *word);

////////////////////////////////////////////////////////////////////////
// TO NOTE
// Apparently it is not a native function in C11 at strings.h,
// so I had to make a custom one for compliation, as linking strings
// to pointers in structs was only possible this way.
char *strdup(const char *str) {
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if (dup == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    if(dup)
    {
        strcpy(dup, str);
    }
    return dup;
}

////////////////////////////////////////////////////////////////////////
// Constructor and Destructor

// Creates LinkedList
LL LinkedListNew(void) {
    LL ll = malloc(sizeof(LL));
    if (ll == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    ll->head = NULL;
    ll->tail = NULL;
    return ll;
}

LLNode LinkedListNewNode(void) {
    LLNode ln = malloc(sizeof(LLNode));
    if (ln == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    ln->key = NULL;
    ln->next = NULL;
    return ln;
}

void LinkedListInsert(LL ll, char *key) {
    LLNode new_head = LinkedListNewNode();
    new_head->key = strdup(key);
    if (ll->head == NULL) {
        new_head->next = LinkedListNewNode();
        ll->head = new_head;
        ll->tail = new_head->next;
    } else {
        LLNode old_last_node = ll->tail;
        old_last_node->key = strdup(key);
        old_last_node->next = LinkedListNewNode();
        ll->tail = old_last_node->next;
    }
}

// Creates a new empty tree
Avl TreeNew(void) {
    Avl t = malloc(sizeof(*t));
    if (t == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    t->root = NULL;
    t->total_word_count = 0;
    t->keys = LinkedListNew();
    return t;
}

// Frees all memory associated with the given tree
void TreeFree(Avl t) {
    doFree(t->root);
    free(t);
}

static void doFree(AvlNode n) {
    if (n != NULL) {
        doFree(n->left);
        doFree(n->right);
        free(n);
    }
}

////////////////////////////////////////////////////////////////////////
// Insertion

// Inserts  a  copy of the given time into the tree if it is not already
// in the tree
void TreeInsert(Avl t, char *word) {
    t->total_word_count = t->total_word_count + 1;
    t->root = doInsert(t, t->root, word);
}

static AvlNode doInsert(Avl t, AvlNode n, char *word) {
    
    if (n == NULL) {
        LinkedListInsert(t->keys, word);
        return newNode(word);
    }

    // insert recursively
    int cmp = strcmp(word, n->word);
    if (cmp < 0) {
        n->left = doInsert(t, n->left, word);
    } else if (cmp > 0) {
        n->right = doInsert(t, n->right, word);
    } else { // (cmp == 0)
        // if time is already in the tree,
        // we can return straight away
        n->word_count += 1;
        return n;
    }

    // insertion done
    // fix the height of the current subtree
    n->height = 1 + max(height(n->left), height(n->right));
    
    // rebalance the tree
    if (n->right != NULL && n->left != NULL) {
        int r_height = n->right->height;
        int l_height = n->left->height;
        if (l_height - r_height > 1) {
            if (strcmp(word, n->word) > 0) {
                n->left = rotateLeft(n->left);
            }
            n = rotateRight(n);
        } else if (r_height - l_height > 1) {
            if (strcmp(word, n->word) < 0) {
                n->right = rotateRight(n->right);
            }
            n = rotateLeft(n);
        }
    }
    return n;
}

// Create new node for AVL tree.
static AvlNode newNode(char *word) {
    AvlNode n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    n->word = strdup(word);
    n->height = 0;
    n->word_count = 1;
    n->left = NULL;
    n->right = NULL;
    return n;
}

// Rotates  the  given  subtree left and returns the root of the updated
// subtree.
static AvlNode rotateLeft(AvlNode n) {
    if (n == NULL) {
        return n;
    }
    AvlNode new_root_node = n->right;
    if (new_root_node == NULL) {
        return n;
    }
    n->right = new_root_node->left;
    new_root_node->left = n;
    n->height = max(height(n->left), height(n->right)) + 1;
    new_root_node->height = max(height(new_root_node->left), height(new_root_node->right)) + 1;
    return new_root_node;
}

// Rotates the given subtree right and returns the root of  the  updated
// subtree.
static AvlNode rotateRight(AvlNode n) {
    if (n == NULL) {
        return n;
    }
    AvlNode new_root_node = n->left;
    if (new_root_node == NULL) {
        return n;
    }
    n->left = new_root_node->right;
    new_root_node->right = n;
    n->height = max(height(n->left), height(n->right)) + 1;
    new_root_node->height = max(height(new_root_node->left), height(new_root_node->right)) + 1;
    return new_root_node;
}

// Returns  the height of a subtree while assuming that the height field
// of the root node of the subtree is correct
static int height(AvlNode n) {
    if (n == NULL) {
        return -1;
    } else {
        return n->height;
    }
}

static int max(int a, int b) {
    return a > b ? a : b;
}

////////////////////////////////////////////////////////////////////////
// Searching

AvlNode doSearch(AvlNode n, char *key) {
    if (n == NULL) {
        return n;
    }
    int cmp = strcmp(key, n->word);
    if (cmp < 0) {
        return doSearch(n->left, key);
    } else if (cmp > 0) {
        return doSearch(n->right, key);
    }
    return n;
}


AvlNode TreeSearch(Avl t, char *key) {
    return doSearch(t->root, key);
}

////////////////////////////////////////////////////////////////////////
// Printing (i.e. Debugging)

//////////////////
// List the items

static void doList(AvlNode n);

void TreeList(Avl t) {
    doList(t->root);
}

static void doList(AvlNode n) {
    if (n != NULL) {
        doList(n->left);
        printf("%s %d\n", n->word, n->word_count);
        doList(n->right);

    }
}

///////////////////////////
// Show the tree structure

typedef unsigned long long uint64;
static void doShow(AvlNode n, int level, uint64 arms);

void TreeShow(Avl t) {
    if (t->root != NULL && t->root->height >= 64) {
        printf("Avl is too tall!\n");
    } else {
        doShow(t->root, 0, 0);
    }
}

// This  function  uses a hack to determine when to draw the arms of the
// tree and relies on the tree being reasonably balanced. Don't  try  to
// use this function if the tree is not an AVL tree!
static void doShow(AvlNode n, int level, uint64 arms) {
    if (n == NULL) return;

    printf("%s %d", n->word, n->word_count);
    printf(" (height: %d)\n", n->height);

    if (n->left != NULL) {
        for (int i = 0; i < level; i++) {
            if ((1LLU << i) & arms) {
                printf("│     ");
            } else {
                printf("      ");
            }
        }
        printf("%s", n->right != NULL ? "┝━╸L: " : "┕━╸L: ");
        if (n->right != NULL) {
            arms |= (1LLU << level);
        } else {
            arms &= ~(1LLU << level);
        }
        doShow(n->left, level + 1, arms);
    }

    if (n->right != NULL) {
        // if (n->left != NULL) {
        //     for (int i = 0; i <= level; i++) {
        //         if ((1LLU << i) & arms) {
        //             printf("│     ");
        //         } else {
        //             printf("      ");
        //         }
        //     }
        //     printf("\n");
        // }
        for (int i = 0; i < level; i++) {
            if ((1LLU << i) & arms) {
                printf("│     ");
            } else {
                printf("      ");
            }
        }
        printf("┕━╸R: ");
        arms &= ~(1LLU << level);
        doShow(n->right, level + 1, arms);
    }
}