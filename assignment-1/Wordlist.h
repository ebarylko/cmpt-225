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

void copy_word(const Node& node) {
  Node* tmp = new Node(node);
  this->last->next = tmp;
  tmp->prev = this->last;
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
    Node* cursor = head;
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
    for (Node* curr = head; curr < head + size; curr++) {
      if (curr->word == word) {
        return true;
      }
    }
    return false;
  }

  bool at_end(Node* node) { return !node->next; }

  bool same_word(Node* curr, string const& word) { return curr->word == word; }

  Node* make_node(string const& word) {
    Node* new_word = new Node;
    new_word->count = 1;
    new_word->word = word;
    return new_word;
  }

  bool last_word_matches(Node* node, string const& word) {
    return at_end(node) && same_word(node, word);
  }

  void add_word(const string& word) {
    if (is_empty()) {
      Node* node = make_node(word);
      head = node;
      size++;
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
    }
  }

  void first() { cout << head << endl; }

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
