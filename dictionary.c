/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

bool freenode(node* free_node, int pos);

unsigned long wordcount = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // start at the first chararcter in the word
    int i = 0;
 
    // set the current node to the top of the trie structure
    node* cur = root;

    // set the letter index to a number between 0 and 26            
    int ltr  = getIndex(word[i]);
  
     
    // for each character in a string
    while (word[i] != '\0')
    { 

        // if a node for the letter is null
        if (cur->children[ltr] == NULL)
        {
            // the word is mispelled, clean up and return false
            cur = NULL;
            free(cur);
            return false;
        }
        else
        {
            // go to the child node at the current postion
            cur = cur->children[ltr];
        }

        // move to the next letter in word and set the node index to the letter
        i++;
        ltr = getIndex(word[i]);
    }
    
    // if the current node is_word is true, it is in the dictionary
    if (cur->is_word)
    {
        // cleanup and return true for end of node    
        cur = NULL;
        free(cur);
        return true;        
    }

    // the word was not found
    else
    {
        // cleanup and return false    
        cur = NULL;
        free(cur);
        return false;    
    }
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // define vars
    char line[LENGTH + 1];
    FILE* infile = fopen(dictionary, "r");
    node* new_node = NULL;
    node* cur = NULL;
    
    // if no file opened, return an error.
    if (infile == NULL)
    {
        return 2;
    }
    
    // while lines to read
    while(fscanf(infile, "%s", line) != EOF)
    {      

        // start at the first chararcter in the word
        int i = 0;

        // set the current node to the top of the trie structure
        cur = root;

        // set the letter index to a number between 0 and 26        
        int ltr  = getIndex(line[i]);

        // if this is an empty trie, intialize and set 
        // children to null.  Set current node to root
        if (root == NULL)
        {
            // allocate space for the root node and
            // set all nodes to null
            root = malloc(sizeof(node));
            for(int k = 0; k < 27; k++)
            {
                root->children[k] = NULL;
            }

            // update the current node to point to the initiated root
            cur = root;
        }
        
        // for each character in a string
        while (line[i] != '\0')
        {            
            // if a node for the letter is null   
            if (cur->children[ltr] == NULL)
            {
                // create a new node and set all child nodes to null
                new_node = malloc(sizeof(node));
                for(int k = 0; k < 27; k++)
                {
                    new_node->children[k] = NULL;
                }           

                // if it is an end of a string, set the node is_word to true
                if (line[i + 1] == '\0')
                {
                    new_node->is_word = true;
                }
                else
                {
                    new_node->is_word = false;                   
                }
                
                // set the child node for the current letter to the new node
                cur->children[ltr] = new_node;

                // move to the current node to the new child node
                cur = cur->children[ltr];
            }
            // if the child node is not null for the letter, move to the node
            else
            {
                cur = cur->children[ltr];
            }

            // move to the next letter in word and set the 
            // node index to the letter
            i++;
            ltr = getIndex(line[i]);
         }
        // a word was found, add 1 to the counter
        wordcount++;
    }

    // cleanup    
    new_node = NULL;
    free(new_node);
    cur = NULL;
    free(cur);
    fclose(infile);

    // that's all folks
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // return the current wordcount 
    return wordcount;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // call freenode and pass the root of the trie and
    // start with the last position.
    return freenode(root, 26);
}


/**
 * Unloads a node from memory starting at the given position.  
 * Returns true if successful else false.
 */
bool freenode(node* free_node, int pos)
{

    // check if the last position has not been reached
    if (pos >= 0)
    {
        // if the child node is not null        
        if (free_node->children[pos] != NULL)
        {
            // call freenode recursively to free the child node starting 
            // at the last position
            freenode(free_node->children[pos], 26);
            
            // free the child node
            free(free_node->children[pos]);
        }
       
        // if parent node is not null        
        if (free_node != NULL)
        {
            // call freenode recursively to free node starting 
            // one position (letter) before the current postion
            freenode(free_node, pos - 1);
        }
     }

    // if the free node is the same as root and all postions have been
    // checked, free the root node
    if (free_node == root && pos == 0)
    {
        free(root);
    }
    
    // that's all folks
    return true;
}    

/**
 * Gets the postion in an array between 0 and 26 for a letter
 * a or A equals zero for an array, and "'" is the last position 
 * Returns an int for the position of the letter in the alphabet
 */
int getIndex(char letter)
{
    int index_val;
    if (letter == '\'')
    {
        // if the character is "'", index is 26
        index_val = 26;
    }
    else
    {
        // convert the letter to lowercase if needed, and subtract 
        // the int value of 'a' from the character passed in
        index_val = tolower(letter) - 'a';
    }   
    
    // return the calculated index value
    return index_val;  
}

