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

#include <vector>
#include <algorithm>
#include <functional>
#include <utility>



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



  typedef pair<string, int> WordInfo;

  struct WordQueue {
    WordQueue() : start(0), end(0), size(0){};

    ~WordQueue() {
      while (this->is_not_empty()) {
        this->rest();
      }
    }

    struct WordNode {
      WordNode(string word, int occurences) : word(word), count(occurences), next(0) {};

      string word;
      int count;
      WordNode* next;
    };

    WordNode *start;
    WordNode *end;
    int size;

    /**
     * @brief Takes a word and adds it to the queue
     *
     * @param node the word to add
     */
    void add_word(Node *node)
    {
      WordNode* nd = new WordNode(node->word, node->count);
      if (this->is_empty())
      {
        this->start = this->end = nd;
      }

      this->end->next = nd;
      this->end = nd;
      this->size++;
    }

    /**
     * @brief Returns true if the queue is empty. False otherwise
     *
     * @return true if the queue is empty
     * @return false otherwise
     */
    bool is_empty() const {
      return this->size == 0;
    }

    /**
     * @brief Returns true if the queue is not empty. False otherwise
     *
     * @return true if the queue is not empty
     * @return false otherwise
     */
    bool is_not_empty () const {
      return !this->is_empty();
    }

    /**
     * @brief Returns the number of elements in the list
     * 
     * @return int the number of elements in the list
     */
    int num_of_elements() const {
      return this->size;
    }

    /**
     * @brief Returns the first element of the queue
     *
     * @return WordNode* the first element
     */
    WordNode *peek() const {
      return this->start;
    }

    /**
     * @brief Returns everything in the queue after the first element
     *
     */
    void rest()
    {
      WordNode *removed = this->start;
      this->start = removed->next;

      removed->next = 0;
      delete removed;
      this->size--;
    }

    /**
     * @brief Returns all the words in the list ordered alphabetically
     * 
     * @return vector<WordInfo> the words in the list in alphabetical order
     */
    vector<WordInfo> all_words() {
      vector<WordInfo> words;
      while (this->is_not_empty())
      {
        WordNode* word = this->peek();
        words.push_back(WordInfo(word->word, word->count));
        this->rest();
      }

      return words;
    }

    /**
     * @brief Prints all the words in the queue
     * 
     */
    void print_words() {
      int line_num = 1;
      string word_start = ". { ";
      while (this->is_not_empty())
      {
        WordNode *curr_word = this->peek();
        string word = "\" " + curr_word->word + "\"";
        string end = ", " + to_string(line_num) + "}";
        string line = word_start + word + end;
        cout << line << endl;

        line_num += 1;
        this->rest();
      }
    }
  };

/**
 * @brief Takes a node and returns true if it has a right subtree
 * 
 * @param node the node given
 * @return true if the node has a right subtree
 * @return false otherwise
 */
bool has_right_subtree(Node* node) const {
  return node->right;
}

/**
 * @brief Takes a node and returns the smallest node in the 
 * left subtree
 * 
 * @param start the node to start from 
 * @return Node* the smallest node in the left subtree
 */
Node* find_smallest(Node* start) const {
  Node* smallest = start;

  while (smallest && smallest->left) {
    smallest = smallest->left;
  }

  return smallest;
}

/**
 * @brief Takes a node and moves upwards until it finds a node which is a left child.
 * Returns the parent of the discovered node if it exists, null otherwise.
 * 
 * @param node the node to start from
 * @return Node* the parent of the discovered node
 */
Node* find_left_child_parent(Node* node) const {
  Node* curr = node;

  while (curr->parent && !is_left_child_of(curr, curr->parent)) {
    curr = curr->parent;
  }

  return curr->parent;
}

/**
 * @brief Takes a node and finds the next node in an inorder traversal
 * 
 * @param curr the current node
 * @return Node* the next node in an inorder traversal sequence
 */
Node* next_node(Node* curr) const {
    if (has_right_subtree(curr)) {
      return find_smallest(curr->right);
    }

    if (!curr->parent || is_left_child_of(curr, curr->parent)) {
      return curr->parent;
    }

    return find_left_child_parent(curr);
}

/**
 * @brief Takes a list and returns the inorder traversal of the list
 * 
 * @return vector<WordInfo> the inorder traversal of the list
 */
vector<WordInfo> inorder_traverse() {
  WordQueue q;
  Node* curr = find_smallest(this->root);

  while (curr) {
    q.add_word(curr);
    curr = next_node(curr);
  }

  return q.all_words();
}

typedef tuple<int, int, string, int, float> ListData;

ListData data() const {
  float percent_of_singletons = 100.0 * num_singletons() / num_different_words();
  return ListData{this->num_different_words(), this->total_words(), this->most_frequent(), 
  this->num_singletons(), percent_of_singletons};
}

/**
 * @brief Takes all the words in the list and prints them in alphabetical order
 * with their associated occurences
 * 
 */
void print_words() const{
    WordQueue q;

    Node *curr = find_smallest(this->root);

    while (curr) {
      q.add_word(curr);
      curr = next_node(curr);
    }

    q.print_words();

}

    /**
     * @brief This represents the root of the tree
     * and contains information about the 
     * number of total, unique, and singleton words
     * 
     */
    struct RootNode : Node {
        RootNode() : Node() {};

        RootNode(const string& word)  {
          this->count = 1;
          this->word = word;
          this->all_words = this->different_words = this->single_words = 1;
          this->most_frequent = this;
        }

        int different_words;
        int all_words;
        int single_words;
        Node* most_frequent;
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
    bool is_left_child_of(Node* child, Node* parent) const {
      return parent->left == child;
    }






    //
    // IMPORTANT: root is the only variable that can be defined in this class.
    // It should point to the top node of your AVL tree. When root == nullptr,
    // the tree is empty.
    //
    // No variables other than root are permitted!
    //
   WordlistTest() {};

   WordlistTest(const string& filename) {
    string word;
    ifstream file(filename);
    /**
     * @brief Add the words to the list while there is input
     * to precess
     * 
     */

      while (file >> word) {
        this->add_word(word);
      }

      file.close();
   }

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
    return new RootNode(word);
}


/**
 * @brief Takes a node and returns a root node 
 * which has the information about the state of the list
 * and the information unique to the node given
 * 
 * @param new_root the node given
 * @return RootNode* a root node with the information about the state of the list
 * and the information solely found in the node passed
 */
RootNode* replace_root_with_info_from(Node& new_root) {
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
Node* add_child(Node* target, const string& word) {
  Node* child = mk_node(word);
  if (target->word > word) {
    target->left = child;
  } else {
    target->right = child;
  }
  
  child->parent = target;

  this->root->different_words++;
  this->root->single_words++;


  return child;
}


/**
 * @brief Takes two nodes n1 and n2 and compares the difference of their occurences in the list.
 * Returns -1 if the difference is negative, 0 if it is 0, and 1 if it is positive.
 * 
 * @param node the first node 
 * @param rt the second node
 * @return int a value which indicates if the difference of the occurences of both nodes
 * is negative, 0, or positive
 */
int compare_word_counts(Node& node) {
 int difference = node.count - this->root->most_frequent->count;
 return difference < 0 ? -1 : difference == 0 ?  0 : 1;

}

/**
 * @brief Takes a node N and the root and updates the most frequent word in the list
 * if N appears the most often
 * 
 * @param node the node being checked
 * @param rt the root 
 */
void update_most_frequent_word(Node& node) {
  switch (compare_word_counts(node))
  {
  case -1:
    break;

  case 0:
    // When two words have the same frequency, the smaller word is chosen
    this->root->most_frequent = this->root->word > node.word ? &node : this->root;
    break;

  default:
    this->root->most_frequent = &node;
  }
}

/**
 * @brief Takes a node and updates the Wordlist with information
 * about the total number of words and the number of unique words
 * 
 * @param node the node given
 */
void increase_word_count(Node& node) {
   // Reducing the amount of singletons if the word is duplicated
  if (node.count == 1) {
    this->root->single_words--;
  }

  node.count++;

  update_most_frequent_word(node);
}

int largest_child_height(Node* child) {
    if (!child) {
      return -1;
    }

    int right_height = child->right_height;
    int left_height = child->left_height;
    int updated_height =  left_height > right_height ? left_height : right_height;
    return updated_height;
}

/**
 * @brief Takes a parent and a child and updates the heights of the children for the parent
 * @param parent the parent node given
 * @param child the child node given
*/
void update_height_of_parent(Node* parent, Node* child) { 
    if (is_left_child_of(child, parent)) {
      update_left_height_of_parent(parent, child);
    } else {
      update_right_height_of_parent(parent, child);
    }
 }

void update_right_height_of_parent(Node* parent, Node* child) { 
    int updated_height = 1 + largest_child_height(child);
      parent->right_height = updated_height;
 }

void update_left_height_of_parent(Node* parent, Node* child) { 
    int updated_height = 1 + largest_child_height(child);
      parent->left_height = updated_height;
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
RotationType left_rotation_type(Node* node) {
  Node* child = node->left;
  Node* taller_grandchild = child->left_height > child->right_height ? child->left : child->right;
  return is_left_child_of(taller_grandchild, child) ? RotationType::left : RotationType::left_right ;
}

/**
 * @brief Takes a node and returns the rotation which must be applied in order to balance the tree
 * @param node the first unbalanced node
*/
RotationType right_rotation_type(Node* node) {
  Node* child = node->right;
  Node* taller_grandchild = child->left_height > child->right_height ? child->left : child->right;
  return is_left_child_of(taller_grandchild, child) ? RotationType::right_left : RotationType::right ;
}



/**
 * Takes a node and returns the type of rotation which must be performed
 * @param node the node given
*/
RotationType rotation_type(Node* node) {
  return is_imbalanced_on_left(node) ? left_rotation_type(node) : right_rotation_type(node);
}

/**
 * @brief Takes a set of words and a current node in the tree and adds all the
 * words in the tree to the set using inorder traversal
 * 
 * @param words the set of words
 * @param node the current node in the tree
 * @return vector<string> the collection of words in the tree
 */
void inorder_traversal(vector<string>& words, Node* node) {
    if (!node) {
      return;
    }
    inorder_traversal(words, node->left);
    words.push_back(node->word);
    return inorder_traversal(words, node->right);
}


vector<string> words_in_order() {
  vector<string> words;
  inorder_traversal(words, this->root);
  return words;
}

typedef pair<int, int> subtree_heights;
typedef vector<subtree_heights> heights;

/**
 * @brief Takes a collection and a node and returns 
 * a collection with the heights of all the nodes in the tree
 * visited using inorder traversal
 * 
 * @param heights the collection of heights
 * @param node the node given
 */
void inorder_height_traversal(heights& heights, Node* node) {
    if (!node) {
      return;
    }

    inorder_height_traversal(heights, node->left);
    heights.push_back(pair<int, int>{node->left_height, node->right_height});
    return inorder_height_traversal(heights, node->right);
}

heights all_heights() {
  heights heights;
  inorder_height_traversal(heights, this->root);
  return heights;
}

void connect_child_to(Node* child, Node* parent) {
  if (child) {
    child->parent = parent;
    
  }
}

/**
 * @brief Takes a parent and a child and connects the child to its parent
 * 
 * @param parent the parent given
 * @param child the child given
 */
void connect_child_to_parent(Node* parent, Node* child) {
  assert(child);

   // Checking if the child is the root
  if (!parent) {
    child->parent = nullptr;
    return;
  }

   // Connecting the child and parent nodes together
  child->parent = parent;
  
  if(parent->word > child->word) {
    parent->left = child;
  } else {
    parent->right = child;
  }

}


/**
 * @brief Takes a node and changes it to a root node, adding to it the 
 * information about the state of the list from the original root
 * 
 * @param new_parent the node to convert
 */
void transform_child_into_root(Node *new_parent)
{
  //  Creating the new root and connecting it to its children
  RootNode *updated_root = new RootNode(*this->root);
  (Node &)*updated_root = *new_parent;

  new_parent->right = new_parent->left = new_parent->parent = nullptr;
  this->root = updated_root;

  updated_root->right->parent = updated_root;
  updated_root->left->parent = updated_root;

  delete new_parent;
}

vector<int> list_info() {
  vector<int> info{this->num_different_words(), this->num_singletons(), this->total_words()};
  return info;
}


/**
 * @brief Takes an unbalanced node and balances it with a left rotation
 * 
 * @param node the node to balance
 */
/**
*/
void left_rotation(Node* node) {
  /**
   * @brief Separating the nodes to be moved around
   * 
   */
  Node* child = node->left;
  Node* a = child->right;

  /**
   * @brief Moving the nodes to their correct position
   * 
   */
  child->right = node;
  connect_child_to_parent(node->parent, child);
  node->parent = child;
  node->left = a;


  /**
   * Updating the heights of the shifted nodes.
  */
  update_left_height_of_parent(node, a);
  update_right_height_of_parent(child, node);
  update_height_to_root_from(node);

   // Updating the root with the old information and the new node
  if (is_root(node)) {
    transform_child_into_root(child);
  }
}

/**
 * @brief Takes an unbalanced node 
 * and applies a right rotation to balance the tree 
 * 
 * @param node the node given
 */
void right_rotation(Node* node) {

   // Separating the nodes to be moved around
  Node* child = node->right;
  Node* left_grandchild = child->left;

  //Moving the nodes to their correct position

  child->left = node;
  child->parent = node->parent;

  if (child->parent) {
    if (child->parent->right == node) {
      child->parent->right = child;
    } else {
      child->parent->left = child;
    }
  }
  
  node->parent = child;
  node->right = left_grandchild;

   // Updating the heights of the changed nodes
  update_right_height_of_parent(node, left_grandchild);
  update_left_height_of_parent(child, node);
  update_height_to_root_from(child);

   // Updating the root with the old information and the new node
  if (is_root(node)) {
    transform_child_into_root(child);
  }
}


/**
 * @brief Takes a node N and updates the heights of all the nodes in the path from N to the root
 * 
 * @param child the node to start from
 */
void update_height_to_root_from(Node* child) {
  Node* parent = child->parent;
  while (parent) {
    update_height_of_parent(parent, child);
    parent = parent->parent;
    child = child->parent;
  }
}

/**
 * @brief Takes an unbalanced node 
 * and applies a right left rotation to balance the tree 
 * 
 * @param node the node given
 */
void right_left_rotation(Node* node) {
  /**
   * @brief Separating the nodes to be moved around
   * 
   */
  Node* child = node->right;
  Node* left_grandchild = child->left;
  child->left = 0;

  /**
   * @brief Arranging the nodes for a right rotation
   * 
   */
  connect_child_to_parent(node, left_grandchild);
  connect_child_to_parent(left_grandchild, child);

  update_height_of_parent(child, child->left);
  update_height_of_parent(left_grandchild, child);

  right_rotation(node);
}

/**
 * @brief Takes an unbalanced node and applies a left right rotation on it so 
 * the tree is balanced afterwards
 * 
 * @param node the unbalanced node given
 */
void left_right_rotation(Node* node) {
  Node* child = node->left;
  Node* grand_child = child->right;
  child->right = 0;

  /**
   * @brief Creating an unbalanced subtree which needs a left rotation
   * 
   */
  connect_child_to_parent(node, grand_child);
  connect_child_to_parent(grand_child, child);

  update_right_height_of_parent(child, child->right);
  update_left_height_of_parent(grand_child, child);
  left_rotation(node);
}

/**
 * @brief Takes an unbalanced node and applies one of the four following rotations
 * on it depending on the type of imbalance: 
 * left rotation, right rotation, left right rotation, right left rotation
 * 
 */
void trinode_rotation(Node*& node) {
  switch (rotation_type(node)) {

    case left: 
    left_rotation(node);

    break;

    case right:
    right_rotation(node);
    break;

    case left_right:
    left_right_rotation(node);
    break;
    
    case right_left: 
    right_left_rotation(node);
  }
  
}

/**
 * @brief Takes a node and returns true if it is not the root. False otherwise
 * @param node the node given to check
*/
bool is_not_root(Node* node) {
  return node != this->root;
}

/**
 * @brief Takes a node and returns true if it is the root. False otherwise
 * 
 * @param node the node given
 * @return true if the node is the root
 * @return false otherwise.
 */
bool is_root(Node* node) {
  return !is_not_root(node);
}

/**
 * @brief Takes a starting node and moves up the tree looking for an unbalanced 
 * node. Returns the node if one is found, and null otherwise
 * 
 * @param start the node to start from
 * @return Node* the unbalanced node
 */
Node* find_unbalanced_node(Node* start) {
  Node* curr = start;
  while (height_diff_less_than_2(curr) && is_not_root(curr)) {
    curr = curr->parent;
  }
  return height_diff_less_than_2(curr) ? 0 : curr;
}

/**
 * @brief Takes a node ND and moves upward from ND, rebalancing the tree if needed
 * @param start the node to start at
*/
// void rebalance_tree(Node*& start) {
//   Node* curr = start;
//   Node* prev;
//   /**
//    * Adjusting the heights of the nodes
//   */
//   while (is_not_root(curr)) {
//     prev = curr;
//     curr = curr->parent;
//     update_height_of_parent(curr, prev);
//   }

//   Node* target = find_unbalanced_node(start);

//   /**
//    * Do nothing if tree is balanced
//   */
//   if (!target) {
//     return;
//   }

//   trinode_rotation(target);
// }

/**
 * @brief Takes a node N and rotates the tree if any node above N is unbalanced.
 * Does nothing otherwise
 * 
 * @param node 
 */
void rotate_tree(Node* node) {
  /**
   * @brief Finding the unbalanced node
   */
  Node* target = find_unbalanced_node(node);

  /**
   * Do nothing if tree is balanced
  */
  if (!target) {
    return;
  }

  /**
   * @brief  Rotate the tree so it becomes balanced
   */
  trinode_rotation(target);
}

/**
 * @brief Takes a node and does nothing 
 * 
 * @param node the node given
 */
void no_effect(Node* node) {
  node->count += 0;
}

/**
 * @brief Takes a node and rebalances the tree after adding the node
 * 
 * @param node the node to add
 */
void rebalance_tree(Node* node) {
  update_tree(node, bind(&WordlistTest::rotate_tree, this, placeholders::_1));
}

/**
 * @brief Takes a node and adds it to the tree without rebalancing
 * the tree afterwards
 * 
 * @param node the node to add
 */
void unbalanced_insertion(Node* node) {
  update_tree(node, bind(&WordlistTest::no_effect, this, placeholders::_1));
}

void update_tree(Node* start, function<void(Node*)> f) {
  Node* curr = start;
  Node* prev;
  /**
   * Adjusting the heights of the nodes
  */
  while (is_not_root(curr)) {
    prev = curr;
    curr = curr->parent;
    update_height_of_parent(curr, prev);
  }

  f(start);
}


/**
 * @brief Takes a word and balances the list after adding it
 * 
 * @param word the word to add
 */
void add_word(const string& word) {
  add_word_using_f(word, bind(&WordlistTest::rebalance_tree, this, placeholders::_1));
}

/**
 * @brief Takes a word and adds it to the list
 * without balancing the list after
 * 
 * @param word the word to add
 */
void unbalanced_word_insertion(const string& word) {
  add_word_using_f(word, bind(&WordlistTest::unbalanced_insertion, this, placeholders::_1));
}

/**
 * @brief Takes a word and a function and applies the function to the 
 * word after adding it to the list
 * 
 * @param word the word to add
 * @param f the function to apply
 */
void add_word_using_f(const string& word, function<void(Node*)> f) {
  // Set root of tree if it is empty
  if (!this->root) {
    this->root = new RootNode(word);
    return;
  }

  this->root->all_words++;

  Node* target = find_word_or_parent(word);

  // Adjust the number of occurences for the word if it is in the list. 
  if (target->word == word) {
    increase_word_count(*target);
  } else {
   // Insert the word into its position and rebalance the tree if necessary
    Node* child = add_child(target, word);
    update_most_frequent_word(*child);
    // f(child);
    if (58 > 100) {
      f(child);
    }
  }
}

/**
 * @brief Takes a collection of words and adds each one to the list
 * 
 * @param words the collection of words to add
 */
void add_n(initializer_list<string> words) {
  for_each(words.begin(), words.end(), bind(&WordlistTest::add_word, this, placeholders::_1));
}

/**
 * @brief Takes a collection of words and adds each one to the list
 * without rebalancing the list afterwards
 * 
 * @param words the list of words to add
 */
void unbalanced_add_n(initializer_list<string> words) {
  for_each(words.begin(), words.end(), bind(&WordlistTest::unbalanced_word_insertion, this, placeholders::_1));
}


/**
 * @brief Takes a word and returns the node corresponding to the 
 * word if it is in the list. Returns null otherwise.
 * 
 * @param word the word to search for
 * @return Node* the node corresponding to the word being searched for
 */
Node* find_word_or_parent(const string& word) const {
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

  // Return the node if it is in the list. Otherwise, return the parent
  assert(curr || parent);
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
  Node* target = find_word_or_parent(w);
  return target->word == w ? target->count : 0;
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
 * @brief Returns the most frequent word in the list along with its number of occurences
 * 
 * @return string the most frequent word in the list
 */
string most_frequent() const {
  Node* most_frequent = this->root->most_frequent;
  return most_frequent->word + " " +  to_string(most_frequent->count);
}

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
// void increase_word_count(Node* node) {
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

//   Node* target = find_word_or_parent(word);
//   /**
//    * @brief Adjust the number of occurences for the word if
//    * it is in the list. 
//    * 
//    */
//   if (target) {
//     target->count++;
//     increase_word_count(target);
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
// Node* find_word_or_parent(const string& word) const {
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
//   Node* target = find_word_or_parent(w);
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
