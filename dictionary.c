/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
#include "trie.h"

void clearDictionary(node *cursor);
void initToNull(node *nodeToInit);

node* root;

const int ALPHA_CONV = 97;
int wordCount = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // set root node of trie
    node* cursor = root;
    int length = strlen(word);
    // iterate through word
    for (int i = 0; i < length; i++)
    {
        int alpha_index = word[i];
        
        // if uppercase, convert to lowercase
        if (word[i] >= 65 && word[i] <= 90)
        {
            alpha_index = alpha_index + 32;
        }
        // if ' assign index of 26, else assign alpha index
        alpha_index = (word[i] == 39) ? 26 : alpha_index - ALPHA_CONV;
        
        if (cursor->children[alpha_index] == NULL)
        {
            return false;
        }
        
        // letter exists, advance to next node in trie
        cursor = cursor->children[alpha_index];
        
        // if on last letter
        if ((length - 1) == i)
        {
            if (cursor->is_word == true)
            {
                return true;
            }
            return false;
        }
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    int alpha_index;
    root = malloc(sizeof(node));
    node* cursor = root;
    
    initToNull(root);
    
    FILE *fp = fopen(dictionary, "r");
    int c = fgetc(fp);

    // iterate through end of file
    while (c != EOF)
    {
        // if new line...
        if (c == 10)
        {
            wordCount++;
            cursor->is_word = true;
            cursor = root;
        }
        else
        {
            // if ' assign 26 to index, if not convert ascii to alphabetical index
            alpha_index = (c == 39) ? 26 : c - ALPHA_CONV;
            
            if (cursor->children[alpha_index] == NULL)
            {
                cursor->children[alpha_index] = malloc(sizeof(node));
            }
            // advance to next node in trie
            cursor = cursor->children[alpha_index];
            initToNull(cursor);
        }
        
        // continue to next character
        c = fgetc(fp);
    }
    
    // if no errors while checking file...
    if (ferror(fp) == 0 && fclose(fp) == 0)
    {
        return true;
    }
    
    return false;
}

void initToNull(node *nodeToInit)
{
    for (int i = 0; i < 27; i++)
    {
        nodeToInit->children[i] = NULL;
    }
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordCount;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    clearDictionary(root);
    
    return true;
}

/**
 * unload helper function.
 */
void clearDictionary(node *cursor)
{
    node* next = NULL;
    
    // iterate through cursor->children
    for (int i = 0; i < 27; i++)
    {
        // if cursor->children[i] != null
        if (cursor->children[i] != NULL)
        {
            // traverse cursor
            next = cursor->children[i];
            clearDictionary(next);
        }
    }
    
    free(cursor);
}