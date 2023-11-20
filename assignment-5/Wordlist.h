// Wordlist.h

#pragma once

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : Eitan Barylko
// St.# : 301559349
// Email: eab16@sfu.ca
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

//
// Do not use any other #includes
//
#include "Wordlist_base.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

//
// IMPORTANT: no global variables are permitted in this file!
//
// It is okay to define helper functions defined outside the class.
//

class Wordlist : public Wordlist_base
{
        
    //
    // Use this Node to implement an AVL tree for the word list. You can *add*
    // extra variables/methods/features to this struct if you like, but you must
    // keep its name the same, and also keep the word, count, left, and right
    // variables as defined.
    //
    struct Node
    {
        string word;
        int count;
        Node *left;
        Node *right;
        int left_height;
        int right_height;
    };

    /**
     * @brief This represents the root of the tree
     * and contains information about the 
     * number of total, unique, and singleton words
     * 
     */
    struct RootNode : Node {
        int different_words;
        int all_words;
        int single_words;
    };

    RootNode *root = nullptr;

    //
    // IMPORTANT: root is the only variable that can be defined in this class.
    // It should point to the top node of your AVL tree. When root == nullptr,
    // the tree is empty.
    //
    // No variables other than root are permitted!
    //
   public:
   Wordlist() {};

/**
 * @brief Takes a word and creates a root node which
 * has the same word as the one passed and has no children
 * 
 * @param word the word passed
 * @return RootNode* the root of the tree with the word passed
 */
RootNode* mk_root(const string& word) {
    RootNode* root = new RootNode;
    root->word = word;
    root->all_words = root->different_words = root->single_words = 1;
    root->count = 1;
    root->left_height = root->right_height = 0;
    root->left = 0;
    root->right = 0;

    return root;
}

/**
 * @brief Takes a word and adds it to the list. If it is already there,
 * it increments the number of times it appeared. Otherwise, it adds the
 * word in in it's correct alphabetical position

 * 
 * @param word the word to add
 */
void add_word(const string& word) {
  /**
   * @brief Set root of tree if it is empty
   * 
   */
  if (!root) {
    root = mk_root(word);
    return;
  }

  // Node* target = find_word(word);
  // if (word_has_been_found(target)) {
  //   target->count++;
  // } else {
  //   Node* child = add_child(target, word);
  //   rebalance_tree(child);
  // }
  

}

   /**
    * @brief Returnss the number of singletons in the Wordlist
    * 
    * @return int the number of singletons
    */
   int num_singletons() const {
        if (!root) {
        return 0;
        }
    
        return root->single_words;
   };

   /**
    * @brief Returns the number of different words in the list
    *
    * @return int number of different words in the list
    */
   int num_different_words() const {
     if (!root) {
       return 0;
     }

     return root->different_words;
   }

   /**
    * @brief Returns the number of all the words in the list
    *
    * @return int the number of words in the list
    */
   int total_words() const {
     if (!root) {
       return 0;
     }

     return root->all_words;
   };
};
// class Wordlist

//
// Make sure to thoroughly test your code as you go!
//
