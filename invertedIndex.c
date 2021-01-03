/*
By Navid Bhuiyan
Date: 23/10/2020

This program allows for the information retrieval of finding words in 
files using Inverted BSTs and Linked Lists.

TO NOTE:
    - If we were able to change how certain ADTs were structured,
      I could've made Part 2 more efficient.
    - Hash Tables would've made Part 1 really efficient, but I could
      not code the datatype and COMP2521 has not touched on this ADT.
    - Part 1 and Part 2 could have used better sorting algorithms, as I know
      the sorting algorithms implemented are really simple O(n*m) to O(n^2) loop 
      algorithms (for n and m cases of unique words). 
    - Overall, Part 2 could have been optimised in terms of sorting algorithms.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include "invertedIndex.h"
#include "avltreewordcount.h"

// PART 1 FUNCTIONS
////////////////////////////////////////////////////////////////////////

/* 
InvertedIndexBST Helper Functions
*/

// Generates the word count trees per file for processing.
Avl word_count_tree_generator(char *filename) {
    // Open new word_count_tree
    Avl word_count_tree = TreeNew();
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "Can't open file\n");
        exit(EXIT_FAILURE);
    }
    // Iterate through each word in file and insert it into the AVL tree.
    char word[WORD_LENGTH];
    while (fscanf(f, "%s", word) != EOF) {
        TreeInsert(word_count_tree, normaliseWord(word));
    }
    fclose(f);
    return word_count_tree;
}

// Sort filename list
void sort_filename_list(LL filename_list) {
    if (filename_list != NULL) {
        // Loop through filenname_list and 
        // order word alphabetically.
        char *tmp_filename;
        LLNode head1 = filename_list->head;
        LLNode head2 = filename_list->head;
        int cmp;
        while (head1->key != NULL) {
            while (head2->key != NULL) {
                cmp = strcmp(head1->key, head2->key);
                if (cmp < 0) {
                    tmp_filename = head1->key;
                    head1->key = head2->key;
                    head2->key = tmp_filename;
                }
                head2 = head2->next;
            }
            head2 = filename_list->head;
            head1 = head1->next;
        }
    }
}

// Create FileListNode
FileList insert_filelist_node(void) {
    FileList filelist = malloc(sizeof(FileList));
    if (filelist == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    filelist->filename = NULL;
    filelist->next = NULL;
    filelist->tf = 0.0;
    return filelist;
}

// Create InvertedIndexBST node
InvertedIndexBST insert_inverted_node(void) {
    InvertedIndexBST temp_node = malloc(sizeof(InvertedIndexBST));
    if (temp_node == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    temp_node->fileList = insert_filelist_node();
    temp_node->word = NULL;
    temp_node->left = NULL;
    temp_node->right = NULL;
    return temp_node;
}
// Inserts word info for each word instance inside a document.
InvertedIndexBST insert_inverted_bst(InvertedIndexBST inverted_index_bst, 
                                    AvlNode word_info, int total_word_count, char *filename) {
    
    // Insert new word not seen before.
    if (inverted_index_bst == NULL) {
        inverted_index_bst = insert_inverted_node();
        inverted_index_bst->word = strdup(word_info->word);
        inverted_index_bst->fileList->filename = strdup(filename);
        inverted_index_bst->fileList->tf = ((double) word_info->word_count 
                                            / (double) total_word_count);
        return inverted_index_bst;
    }

    // Recursion until word is found (if it exists in InvertedIndexBST)
    int cmp = strcmp(word_info->word, inverted_index_bst->word);
    if (cmp < 0) {
        inverted_index_bst->left = insert_inverted_bst(inverted_index_bst->left, 
                                                       word_info, total_word_count, filename);
    } else if (cmp > 0) {
        inverted_index_bst->right = insert_inverted_bst(inverted_index_bst->right, 
                                                        word_info, total_word_count, filename);
    // Word exists in InvertedIndexBST.
    } else {
        FileList temp_list = inverted_index_bst->fileList;
        while (temp_list->next != NULL) {
            temp_list = temp_list->next;
        }
        temp_list->next = insert_filelist_node();
        temp_list->next->filename = strdup(filename);
        temp_list->next->tf = (double) word_info->word_count / (double) total_word_count;
    }
    return inverted_index_bst;
}

// Update the InvertedIndexBST as each word in each file is being added/
InvertedIndexBST update_inverted_bst(InvertedIndexBST inverted_index_bst, 
                                     Avl word_count_tree, char *filename) {

    // Get the keys linked list from avltreewordcount data stucture.
    LLNode word_count_tree_keys_list = word_count_tree->keys->head;
    int total_word_count = word_count_tree->total_word_count;
    // If no words are found in empty file.
    if (word_count_tree_keys_list == NULL) {
        return inverted_index_bst;
    }
    // Loop through the keys to add information per word in the InvertedIndexBST
    AvlNode word_info = NULL;
    while (word_count_tree_keys_list->key != NULL) {
        // Find word in word_count_tree Avl tree.
        word_info = TreeSearch(word_count_tree, word_count_tree_keys_list->key);
        // Insert new word information in place for each file.
        inverted_index_bst = insert_inverted_bst(inverted_index_bst, word_info,
                                                 total_word_count, filename);
        // Go through the keys.
        word_count_tree_keys_list = word_count_tree_keys_list->next;
    }
    return inverted_index_bst;
}

/*
Print InvertedIndexBST helper functions
*/

// Print inorder.
void printInvertedIndexHelper(InvertedIndexBST Tree) { 
    // Append to each node to file. 
    FILE *f = fopen("invertedIndex.txt", "a");
    fprintf(f, "%s ", Tree->word);
    FileList temp_filelist = Tree->fileList;
    while (temp_filelist != NULL) {
        fprintf(f, "%s ", temp_filelist->filename);
        fprintf(f, "(%lf) ", temp_filelist->tf);
        temp_filelist = temp_filelist->next;
    }
    fprintf(f, "\n");
    fclose(f);
}

// Recursive function to print InvertedIndexBST.
void printInvertedIndexRecursive(InvertedIndexBST tree) {
    if (tree != NULL) {
        printInvertedIndexRecursive(tree->left);
        printInvertedIndexHelper(tree);
        printInvertedIndexRecursive(tree->right);
    } 
}

/*
Feature Functions
*/

// Normalise words according to spec.
char *normaliseWord(char *str) {
    // Put string to lowercase
    int i = 0;
    int length = strlen(str);
    while (i < length) {
        // Check if char is ascii
        str[i] = tolower(str[i]);
        i++;
    }
    // Check last char is punctuation and change it if it is so
    char last_char = str[length - 1];
    if (last_char == '.' || last_char == ',' || last_char == ';' || last_char == '?') {
        str[length - 1] = '\0';
    }
    return str;
}

InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
    // Extract filenames
    FILE *f = fopen(collectionFilename, "r");
    Avl word_count_tree = NULL;
    InvertedIndexBST inverted_index_bst = NULL;
    // Get filenames
    char filename[WORD_LENGTH];
    LL filename_list = LinkedListNew();
    while (fscanf(f, "%s", filename) != EOF) {
        LinkedListInsert(filename_list, filename);
    }
    fclose(f);
    // Sort filenames for BST insertion (makes life easier for alphabettically sorted FIleLists at
    // each InvertedIndecBST node).
    if (filename_list->head == NULL) {
        return NULL;
    }
    sort_filename_list(filename_list);
    // Loop through each file
    LLNode filename_list_head = filename_list->head;
    while (filename_list_head->key != NULL) {
        // Count words unique to each file in my custom AVL wordcount tree.
        word_count_tree = word_count_tree_generator(filename_list_head->key);
        inverted_index_bst = update_inverted_bst(inverted_index_bst, 
                                                 word_count_tree, filename_list_head->key);
        // Free word_count_tree for a new file to proccess (saving memory).
        TreeFree(word_count_tree);
        word_count_tree = NULL;
        filename_list_head = filename_list_head->next;
    }
    return inverted_index_bst;
}

// Print Inverted Index Tree in Order
void printInvertedIndex(InvertedIndexBST tree) {
    // Overwrite for new file everytime.
    FILE *f = fopen("invertedIndex.txt", "w");
    fseek(f, 0, SEEK_SET);
    fclose(f);
    printInvertedIndexRecursive(tree);
}

// PART 2 FUNCTIONS
////////////////////////////////////////////////////////////////////////

/*
Helper Functions
*/

// Insert TfldList Node
TfIdfList insert_tfidlist_node(void) {
    TfIdfList tfid_node = malloc(sizeof(TreeList));
    if (tfid_node == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    tfid_node->filename = NULL;
    tfid_node->next = NULL;
    return tfid_node;
}


// Combine TfIdfLists for multi-query search.
TfIdfList retrieve_helper_combine_lists(TfIdfList list_to_add_on, 
                                        TfIdfList list_to_extract_from) {
    // Loop variables.
    TfIdfList tmp_list_1 = list_to_extract_from;
    TfIdfList tmp_list_2 = list_to_add_on;
    TfIdfList to_free = NULL;
    while (tmp_list_1 != NULL) {
        // Loop through extraction list to add onto add on list.
        while (tmp_list_2->next != NULL) {
            // If a match is made, add the tfldf sums together
            if (strcmp(tmp_list_1->filename, tmp_list_2->filename) == 0) {
                tmp_list_2->tfIdfSum = tmp_list_2->tfIdfSum + tmp_list_1->tfIdfSum;
                break;
            }
            tmp_list_2 = tmp_list_2->next;
        }
        // If there is no common filename, add a new node to the add on list.
        if (tmp_list_2->next == NULL) {
            tmp_list_2->next = insert_tfidlist_node();
            tmp_list_2->next->tfIdfSum = tmp_list_1->tfIdfSum;
            tmp_list_2->next->filename = tmp_list_1->filename;
            tmp_list_2->next->next = NULL;
        }
        // Free used extract list node to save memory as information is transferred.
        to_free = tmp_list_1;
        tmp_list_1 = tmp_list_1->next;
        tmp_list_2 = list_to_add_on;
        free(to_free);
    }
    return list_to_add_on;
}


// Sort tdfllist (could be more efficient if I implemented QuickSort)
void sort_tflfdlist(TfIdfList tfldflist) {
    // If NULL reuturn the List.
    if (tfldflist != NULL) {
        // Difference threshold for double tdlf
        double threshold = 0.000001;
        // Temp values for sort.
        char *tmp_filename;
        double tmp_tdlf;
        // Sort via tdlf values for descending order.
        TfIdfList head1 = tfldflist;
        TfIdfList head2 = tfldflist;
        while (head1 != NULL) {
            while (head2 != NULL) {
                if (head1->tfIdfSum - head2->tfIdfSum > threshold) {
                    tmp_filename = head1->filename;
                    tmp_tdlf = head1->tfIdfSum;
                    head1->filename = head2->filename;
                    head1->tfIdfSum = head2->tfIdfSum;
                    head2->tfIdfSum = tmp_tdlf;
                    head2->filename = tmp_filename;
                }
                head2 = head2->next;
            }
            head2 = tfldflist;
            head1 = head1->next;
        }
        // Sort filenames in ascending order only if values are within 
        // the threshold.
        head1 = tfldflist;
        head2 = tfldflist->next;
        while (head1 != NULL && head2 != NULL) {
            while (head2 != NULL) {
                if (strcmp(head1->filename, head2->filename) > 0 
                    && head1->tfIdfSum - head2->tfIdfSum < threshold) {
                    tmp_filename = head1->filename;
                    head1->filename = head2->filename;
                    head2->filename = tmp_filename;
                }
                head2 = head2->next;
            }
            head1 = head1->next;
            // Move on with head2 so no double moves happen i.e.
            // only unique matches with different filenames only swap
            // and only occur once.
            head2 = head1->next;
        }
    }
}


// Find word in InvertedIndexBST
InvertedIndexBST search_inverted_index(InvertedIndexBST tree, char *searchWord) {
    if (tree == NULL) {
        return tree;
    }
    int cmp = strcmp(searchWord, tree->word);
    if (cmp < 0) {
        return search_inverted_index(tree->left, searchWord);
    } else if (cmp > 0) {
        return search_inverted_index(tree->right, searchWord);
    }
    return tree;
}

/*
Feature Functions
*/

// CalculateTfIdf for single word
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {
    // Empty files.
    if (tree == NULL) {
        return NULL;
    }
    // Search node (idk if its exact or etc.)
    InvertedIndexBST searched_node = search_inverted_index(tree, searchWord);
    int t = 0;
    FileList temp_filelist = searched_node->fileList;
    while (temp_filelist != NULL) {
        t++;
        temp_filelist = temp_filelist->next;
    }
    // Create new TfIdList for the searched word.
    TfIdfList tfldflist = insert_tfidlist_node();
    TfIdfList head = tfldflist;
    temp_filelist = searched_node->fileList;
    double idf = 0.0;
    double temp_tf = 0.0;
    // Iterate through temp_filelist
    while (temp_filelist != NULL) {
        head->filename = temp_filelist->filename;
        idf = log10((double) D / (double) t);
        temp_tf = temp_filelist->tf;
        head->tfIdfSum = temp_tf * idf;
        // Add new TfIdList node if the temp_filelist
        // isn't terminated.
        if (temp_filelist->next != NULL) {
            head->next = insert_tfidlist_node();
            head = head->next;
        } else {
            head->next = NULL;
        }
        temp_filelist = temp_filelist->next;
    }
    sort_tflfdlist(tfldflist);
    return tfldflist;
}

// CalculateTfIdf for multiple words
TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D) {
    // Create collection list to update constantly.
    TfIdfList collection_list = NULL;
    TfIdfList tmp_list = NULL;
    // Loop through searchWords and build TfId list
    int word_index = 0;
    char *temp_word;
    while (searchWords[word_index] != NULL) {
        temp_word = searchWords[word_index];
        if (collection_list == NULL) {
            collection_list = calculateTfIdf(tree, temp_word, D);
        } else {
            tmp_list = calculateTfIdf(tree, temp_word, D);
            // Combine lists to get overall information about two search words.
            collection_list = retrieve_helper_combine_lists(collection_list, tmp_list);
            tmp_list = NULL;
        }
        word_index++;
    }
    // Sort list
    sort_tflfdlist(collection_list);
    return collection_list;
}
