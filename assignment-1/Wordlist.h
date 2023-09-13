// Wordlist.h

#pragma once

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : Eitan Barylko
// St.# : 301559349
// Email: <put your SFU email address here>
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
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

#include "Wordlist_base.h"

using namespace std;

class Wordlist : public Wordlist_base {
  //
  // Use this Node to implement the doubly-linked list for the word list.
  // Don't change it any way!
  //
  struct Node {
    string word;
    int count;
    Node* next;
    Node* prev;
  };

  int size;
  Node* head;
  bool frozen;
  Node* last;

Node* find_word (const string& target) {
  Node* curr = this->head;
  while (curr && curr->word != target) {
    curr = curr->next;
  }
  return curr;
}

// if this->last is null, then I want to attach the node to the head and the tail of the list, and make the last of the node point to null
// if this->last != null, then I want to assign the next node as the new node. i want to assign the tail to the new node, and have thee new node point back to the last node
/**
 * @brief Takes a node src and copies the information in src to another node 
 * 
 * @param src a const node reference
 */
void copy_word(const Node& src) {
  Node* tmp = new Node(src);

if (this->last) {
  this->last->next = tmp;
  tmp->prev = this->last;
} else {
  this->head = tmp;
  this->head->prev = nullptr;
}
  // this->last->next = tmp;

  this->last = tmp;
  this->last->next = nullptr;
}

 public:
  Wordlist() {
    head = nullptr;
    last = nullptr;
    frozen = (size = 0);
  }

  Wordlist(const Wordlist& source) {
    Node* copy_from = source.head;
    while (copy_from) {
      this->copy_word(*copy_from);
      copy_from = copy_from->next;
    }
    this->size = source.size;
  }

  ~Wordlist() {
    Node* cursor = nullptr;
    cursor = head;
    while (cursor) {
      Node* tmp = cursor->next;
      delete cursor;
      cursor = tmp;
    }
  }

  int length() const { return size; }

  bool is_empty() const { return size == 0; }
  /* const string get_word(int pos) {
  Node* start = head;
  while (pos != 0) {
  head++;
  pos--;
  }
  return start->word;
  }
  */

  bool is_frozen() const { return frozen; }

  bool contains(const string& word) const {
    Node* curr = head;
    while (curr && curr->word != word) {
      curr = curr->next;
    }
    return curr ? true : false;
  }

  bool at_end(Node* node) { return node == last; }

  bool same_word(Node* curr, string const& word) { return curr->word == word; }

  Node* make_node(string const& word) {
    Node* new_word = new Node;
    new_word->count = 1;
    new_word->word = word;
    return new_word;
  }

/**
 * @brief Takes a node and returns true if it is the last item in the list 
 * and it contains the same word as `word`, false otherwise
 * 
 * @param node a Node*
 * @param word a const string referencr
 * @return true if the node passed is the last one in the list and the node contains
 * the word being searched for
 * @return false if condition above not satisfied
 */
  bool last_word_matches(Node* node, string const& word) {
    return at_end(node) && same_word(node, word);
  }

  /**
   * @brief Takes a word and adds it to the list
   * 
   * @param word a const string reference
   */
  void add_word(const string& word) {
    if (is_empty()) {
      Node* node = make_node(word);
      head = node;
      last = node;
      size += 1;
      return;
    }
    Node* curr = head;
    while (!at_end(curr) && !same_word(curr, word)) {
      curr = curr->next;
    }
    if (!at_end(curr) || last_word_matches(curr, word)) {
      curr->count++;
    } else {
      Node* new_word = make_node(word);
      new_word->prev = curr;
      curr->next = new_word;
      last = new_word;
      size += 1;
    }
  }

  string last_word() const { return last->word;}
  string first_word() const {return head->word;}
  // bool last() {
  //   return !tail;
  // }

  //
  // ... your code goes here ...
  //

  //
  // ... you can write helper methods if you need them ...
  //

};  // class Wordlist

//
// ... you can write helper functions here (or before Wordlist) if you need them
// ...
//
