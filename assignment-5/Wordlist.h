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

class WordlistTest : public Wordlist_base {
  public:
    struct Node
    {
        ~Node() {
          delete this->left;
          delete this->right;
        };

        string word;
        int count;
        Node *left;
        Node *right;
        Node* parent;
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

    /**
     * @brief Takes a node and returns true if it is the
     * left child of its parent
     * 
     * @param node the node given
     * @return true if the node is the left child of its parent
     * @return false if the above is not true
     */
    bool is_left_child_of(Node*& child, Node*& parent) {
      return parent->left == child;
    }


  /**
   * @brief Takes a node ND and returns the smallest node starting from ND and looking down 
   * its left subnode. 
   * 
   * @param node the node to start at
   * @return Node* the smallest node found when starting from ND
   */
  Node* find_smallest(Node* node) {
    Node* curr = node;

    while (!curr && curr->left) {
      curr = curr->left;
    }

    return curr;
  }




    //
    // IMPORTANT: root is the only variable that can be defined in this class.
    // It should point to the top node of your AVL tree. When root == nullptr,
    // the tree is empty.
    //
    // No variables other than root are permitted!
    //
   WordlistTest() {};
  ~WordlistTest() {
    delete this->root;
  }


/**
 * @brief Takes a word and creates a root node which
 * has the same word as the one passed and has no children
 * 
 * @param word the word passed
 * @return RootNode* the root of the tree with the word passed
 */
RootNode* mk_root(const string& word) {
    return new RootNode{{word, 1, 0, 0, 0, 0, 0}, 1, 1, 1};
}

/**
 * @brief Takes a word and returns a new node for the word passed
 * 
 * @param word the word given
 * @return Node* the node containing the word given
 */
Node* mk_node(const string& word) {
  return new Node{word, 1, 0, 0, 0, 0, 0};
}

/**
 * @brief Takes a word TARGET and a node SRC and adds TARGET as the right or left
 * child of SRC depending on whether TARGET comes before or after the word
 * SRC contains. Returns the node where TARGET is located.
 * 
 * @param target the node given
 * @param word the word to be added as the right or left child of target
 * @return Node* the node where target is located
 */
Node* add_child(Node*& target, const string& word) {
  Node* child = mk_node(word);
  if (target->word > word) {
    target->left = child;
  } else {
    target->right = child;
  }
  
  child->parent = target;

  this->root->all_words++;
  this->root->different_words++;
  this->root->single_words++;

  return child;
}

/**
 * @brief Takes a node and updates the Wordlist with information
 * about the total number of words and the number of unique words
 * 
 * @param node the node given
 */
void update_wordlist_info(Node*& node) {
  this->root->all_words++;
  /**
   * @brief Reducing the amount of singletons if the word is 
   * duplicated
   * 
   */
  if (node->count == 2) {
    this->root->single_words--;
  }
}

/**
 * @brief Takes a parent and a child and updates the heights of the children for the parent
 * @param parent the parent node given
 * @param child the child node given
*/
void update_height_of_parent(Node*& parent, Node*& child) { 
    int right_height = child->right_height;
    int left_height = child->left_height;

    int updated_height = 1 + ( left_height > right_height ? left_height : right_height );
    if (is_left_child_of(child, parent)) {
      parent->left_height = updated_height;
    } else {
      parent->right_height = updated_height;
    }
 }

/**
 * Takes a node and returns true if the absolute difference of the heights of its children is
 * less than or equal to one.
*/
bool height_diff_less_than_2(Node* node) {
  int height_difference = node->left_height - node->right_height;
  return -1 <= height_difference && height_difference <= 1;
}

enum RotationType{left, right, left_right, right_left};

/**
 * @brief Takes a node and returns true if the difference between the left and right subtree 
 * height is greater than one. False otherwise
* @param node the node given
*/
bool is_imbalanced_on_left(Node* node) const {
  return node->left_height - node->right_height > 1;
}


/**
 * @brief Takes a node and returns the rotation which must be applied in order to balance the tree
 * @param node the first unbalanced node
*/
RotationType left_rotation_type(const Node* node) {
  Node* child = node->left;
  Node* taller_grandchild = child->left_height > child->right_height ? child->left : child->right;
  return is_left_child_of(taller_grandchild, child) ? RotationType::right : RotationType::left_right ;
}

/**
 * @brief Takes a node and returns the rotation which must be applied in order to balance the tree
 * @param node the first unbalanced node
*/
RotationType right_rotation_type(Node*& node) {
  Node* child = node->right;
  Node* taller_grandchild = child->left_height > child->right_height ? child->left : child->right;
  return is_left_child_of(taller_grandchild, child) ? RotationType::right_left : RotationType::left ;
}

/**
 * Takes a node and returns the type of rotation which must be performed
 * @param node the node given
*/
RotationType rotation_type(Node* node) {
  return is_imbalanced_on_left(node) ? left_rotation_type(node->left) : right_rotation_type(node->right);
}

/**
 * @brief Takes a node and rotates the tree so it maintains the height balance property
*/
// bool trinode_rotation(Node*& node) {
//   switch (rotation_type(node)) {

//     case LEFT: 
//     left_rotation(node);

//     break;

//     case RIGHT:
//     right_rotation(node);
//     break;

//     case LEFT-RIGHT:
//     left_rotation(node);
//     right_rotation(node);
    
//     case RIGHT-LEFT: 
//     right_rotation(node);
//     left_rotation(node);
//   }
  
// }

/**
 * @brief Takes a node and returns true if it is not the root. False otherwise
 * @param node the node given to check
*/
bool is_not_root(Node*& node) {
  return node != this->root;
}

/**
 * @brief Takes a node ND and moves upward from ND, rebalancing the tree if needed
 * @param start the node to start at
*/
void rebalance_tree(Node*& start) {
  Node* curr = start;
  Node* prev;
  /**
   * Find imbalanced node while adjusting the heights of the nodes
  */
  while (height_diff_less_than_2(curr) && is_not_root(curr)) {
    prev = curr;
    curr = curr->parent;
    update_height_of_parent(curr, prev);
  }

  /**
   * Do nothing if tree is balanced
  */
  if (height_diff_less_than_2(curr)) {
    return;
  }

  // trinode_rotation(curr);
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
  if (!this->root) {
    this->root = mk_root(word);
    return;
  }

  Node* target = find_word(word);
  /**
   * @brief Adjust the number of occurences for the word if
   * it is in the list. 
   * 
   */
  if (target->word == word) {
    target->count++;
    update_wordlist_info(target);
  } 
  /**
   * @brief Insert the word into its position and rebalance the 
   * tree if necessary
   */
  else {
    Node* child = add_child(target, word);
    rebalance_tree(child);
  }
  
}

/**
 * @brief Takes a word and returns the node corresponding to the 
 * word if it is in the list. Returns null otherwise.
 * 
 * @param word the word to search for
 * @return Node* the node corresponding to the word being searched for
 */
Node* find_word(const string& word) const {
  Node* curr = this->root;
  Node* parent = curr;

/**
 * Looks for the position of the node in the tree
 * Stops when it finds the word or an empty node
*/
  while (curr && curr->word != word) {
    switch ((int)(word > curr->word)) {
      
      case 1: 
      if (curr->right) {
        parent = curr->right;
      } 
      curr = curr->right;
      break;

      default:
      if (curr->left) {
        parent = curr->left;
      }

      curr = curr->left;
      break;
    }

  }

  /**
   * Return the node if it is in the list. Otherwise, return the parent
  */
  return curr ? curr : parent; 

}

/**
 * @brief Takes a word and returns the number of times it 
 * occurs in the list
 * 
 * @param w the word to search for
 * @return int the number of times the word appears in the list
 */
int get_count(const string& w) const {
  Node* target = find_word(w);
  return target ? target->count : 0;
};

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

// class Wordlist : public Wordlist_base
// {
        
//     //
//     // Use this Node to implement an AVL tree for the word list. You can *add*
//     // extra variables/methods/features to this struct if you like, but you must
//     // keep its name the same, and also keep the word, count, left, and right
//     // variables as defined.
//     //
//     struct Node
//     {
//         ~Node() {
//           delete this->left;
//           delete this->right;
//         };

//         string word;
//         int count;
//         Node *left;
//         Node *right;
//         Node* parent;
//         int left_height;
//         int right_height;
//     };

//     /**
//      * @brief This represents the root of the tree
//      * and contains information about the 
//      * number of total, unique, and singleton words
//      * 
//      */
//     struct RootNode : Node {
//         int different_words;
//         int all_words;
//         int single_words;
//     };

//     RootNode *root = nullptr;

//     /**
//      * @brief Takes a node and returns true if it is the
//      * left child of its parent
//      * 
//      * @param node the node given
//      * @return true if the node is the left child of its parent
//      * @return false if the above is not true
//      */
//     bool is_left_child_of(Node* node) {
//       return node->parent->left == node;
//     }

//     /**
//      * @brief Takes a node and removes it from the tree
//      *
//      * @param node the node to remove
//      */
//     void remove_node(Node* node) {
//       if (!node->parent) {
//         return;
//       }

//       if (is_left_child_of(node)) {
//         node->parent->left = 0;
//       } else {
//         node->parent->right = 0;
//       }
//     };

//   /**
//    * @brief Takes a node ND and returns the smallest node starting from ND and looking down 
//    * its left subnode. 
//    * 
//    * @param node the node to start at
//    * @return Node* the smallest node found when starting from ND
//    */
//   Node* find_smallest(Node* node) {
//     Node* curr = node;

//     while (!curr && curr->left) {
//       cout << "The word " << curr->word << endl;
//       curr = curr->left;
//     }

//     return curr;
//   }




//     //
//     // IMPORTANT: root is the only variable that can be defined in this class.
//     // It should point to the top node of your AVL tree. When root == nullptr,
//     // the tree is empty.
//     //
//     // No variables other than root are permitted!
//     //
//    public:
//    Wordlist() {};
//   ~Wordlist() {
//     delete this->root;
//   }


// /**
//  * @brief Takes a word and creates a root node which
//  * has the same word as the one passed and has no children
//  * 
//  * @param word the word passed
//  * @return RootNode* the root of the tree with the word passed
//  */
// RootNode* mk_root(const string& word) {
//     return new RootNode{{word, 1, 0, 0, 0, 0, 0}, 1, 1, 1};
// }

// /**
//  * @brief Takes a word and returns a new node for the word passed
//  * 
//  * @param word the word given
//  * @return Node* the node containing the word given
//  */
// Node* mk_node(const string& word) {
//   return new Node{word, 1, 0, 0, 0, 0, 0};
// }

// /**
//  * @brief Takes a word TARGET and a node SRC and adds TARGET as the right or left
//  * child of SRC depending on whether TARGET comes before or after the word
//  * SRC contains. Returns the node where TARGET is located.
//  * 
//  * @param target the node given
//  * @param word the word to be added as the right or left child of target
//  * @return Node* the node where target is located
//  */
// Node* add_child(Node*& target, const string& word) {
//   Node* child = mk_node(word);
//   if (target->word > word) {
//     target->left = child;
//   } else {
//     target->right = child;
//   }

//   this->root->all_words++;
//   this->root->different_words++;
//   this->root->single_words++;

//   return child;
// }

// /**
//  * @brief Takes a node and updates the Wordlist with information
//  * about the total number of words and the number of unique words
//  * 
//  * @param node the node given
//  */
// void update_wordlist_info(Node* node) {
//   this->root->all_words++;
//   /**
//    * @brief Reducing the amount of singletons if the word is 
//    * duplicated
//    * 
//    */
//   if (node->count == 2) {
//     this->root->single_words--;
//   }
// }

// /**
//  * @brief Takes a parent and a child and updates the heights of the children for the parent
//  * @param parent the parent node given
//  * @param child the child node given
// */
// void update_height_of_parent(Node*& parent, Node*& child) { 
//     if (is_left_child_of(child)) {
//       parent->left_height++;
//     } else {
//       parent->right_height++;
//     }
//  }

// /**
//  * Takes a node and returns true if the absolute difference of the heights of its children is
//  * less than or equal to one.
// */
// bool height_diff_less_than_2(Node*& node) {
//   return  -1 <= node->left_height - node->right_height <= 1;
// }

// enum RotationType{left, right, left_right, right_left};

// /**
//  * @brief Takes a node and returns true if the difference between the left and right subtree 
//  * height is greater than one. False otherwise
// * @param node the node given
// */
// bool is_imbalanced_on_left(Node*& node) {
//   return node->left_height - node->right_height > 1;
// }


// /**
//  * @brief Takes a node and returns true if the difference between the left and right subtree 
//  * height is greater than one. False otherwise
// * @param node the node given
// */
// bool is_imbalanced_on_right(Node*& node) {
//   return node->right_height - node->left_height > 1;
// }

// /**
//  * Takes a node and returns the type of rotation which must be performed
//  * @param node the node given
// */
// RotationType rotation_type(Node*& node) {
//   return is_imbalanced_on_left(node) ? left_rotation_type(node->left) : right_rotation_type(node->right);
// }

// /**
//  * @brief Takes a node and rotates the tree so it maintains the height balance property
// */
// bool trinode_rotation(Node*& node) {
//   switch (rotation_type(node)) {

//     case LEFT: 
//     left_rotation(node);

//     break;

//     case RIGHT:
//     right_rotation(node);
//     break;

//     case LEFT-RIGHT:
//     left_rotation(node);
//     right_rotation(node);
    
//     case RIGHT-LEFT: 
//     right_rotation(node);
//     left_rotation(node);
//   }
  
// }

// /**
//  * @brief Takes a node and returns true if it is not the root. False otherwise
//  * @param node the node given to check
// */
// bool is_not_root(Node*& node) {
//   return node != this->root;
// }

// /**
//  * @brief Takes a node ND and moves upward from ND, rebalancing the tree if needed
//  * @param start the node to start at
// */
// void rebalance_tree(Node*& start) {
//   Node* curr = start;
//   Node* prev;
//   /**
//    * Find imbalanced node while adjusting the heights of the nodes
//   */
//   while (height_diff_less_than_2(curr) && is_not_root(curr->parent)) {
//     prev = curr;
//     curr = curr->parent;
//     update_height_of_parent(curr, prev);
//   }

//   /**
//    * Do nothing if tree is balanced
//   */
//   if (height_diff_less_than_2(curr)) {
//     return;
//   }

//   trinode_rotation(curr);
// }

// /**
//  * @brief Takes a word and adds it to the list. If it is already there,
//  * it increments the number of times it appeared. Otherwise, it adds the
//  * word in in it's correct alphabetical position

//  * 
//  * @param word the word to add
//  */
// void add_word(const string& word) {
//   /**
//    * @brief Set root of tree if it is empty
//    * 
//    */
//   if (!this->root) {
//     this->root = mk_root(word);
//     return;
//   }

//   Node* target = find_word(word);
//   /**
//    * @brief Adjust the number of occurences for the word if
//    * it is in the list. 
//    * 
//    */
//   if (target) {
//     target->count++;
//     update_wordlist_info(target);
//   } 
//   /**
//    * @brief Insert the word into its position and rebalance the 
//    * tree if necessary
//    */
//   else {
//     add_child(target, word);
//     rebalance_tree(child);
//   }
  
// }

// /**
//  * @brief Takes a word and returns the node corresponding to the 
//  * word if it is in the list. Returns null otherwise.
//  * 
//  * @param word the word to search for
//  * @return Node* the node corresponding to the word being searched for
//  */
// Node* find_word(const string& word) const {
//   Node* curr = this->root;
//   Node* parent = curr;

// /**
//  * Looks for the position of the node in the tree
//  * Stops when it finds the word or an empty node
// */
//   while (curr && curr->word != word) {
//     switch (curr->word.compare(word)) {
      
//       case -1: 
//       if (curr->right) {
//         parent = curr;
//       } 
//       curr = curr->right;
//       break;

//       case 1:
//       if (curr->left) {
//         parent = curr = curr->left;
//       }

//       curr = curr->left;
//       break;
//     }

//   }

//   return parent; 

// }

// /**
//  * @brief Takes a word and returns the number of times it 
//  * occurs in the list
//  * 
//  * @param w the word to search for
//  * @return int the number of times the word appears in the list
//  */
// int get_count(const string& w) const {
//   Node* target = find_word(w);
//   return target ? target->count : 0;
// };

// /**
//  * @brief Returnss the number of singletons in the Wordlist
//  *
//  * @return int the number of singletons
//  */
// int num_singletons() const {
//   if (!root) {
//     return 0;
//   }

//   return root->single_words;
//    };

//    /**
//     * @brief Returns the number of different words in the list
//     *
//     * @return int number of different words in the list
//     */
//    int num_different_words() const {
//      if (!root) {
//        return 0;
//      }

//      return root->different_words;
//    }

//    /**
//     * @brief Returns the number of all the words in the list
//     *
//     * @return int the number of words in the list
//     */
//    int total_words() const {
//      if (!root) {
//        return 0;
//      }

//      return root->all_words;
//    };
// };
// class Wordlist

//
// Make sure to thoroughly test your code as you go!
//
