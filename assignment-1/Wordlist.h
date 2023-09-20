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
    Node* next;
    Node* prev;
  };

  int size;
  Node* head;
  bool frozen;
  Node* tail;


  /**
   * @brief Takes a node src and copies the information in src to another node
   *
   * @param src a const node reference
   */

  void append_word(const string& word) {
    Node* tmp = new Node{word, nullptr, nullptr};
    tmp->prev = this->tail;
    if (this->is_empty()) {
      this->head = tmp;
    } else {
      assert(this->tail);
      this->tail->next = tmp;
    }

    this->size++;
    this->tail = tmp;
}

  struct Iterator {
    Node *_current;
    Iterator(Node *node): _current(node) {}

    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Node;
    using pointer = value_type*;
    using reference = value_type&;

    reference operator*() const { return *_current;}
    pointer operator->() { return _current; }

    // Prefix increment
    Iterator& operator++() { _current = _current->next; return *this; }  

    // Postfix increment
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    // Prefix decrement 
    Iterator& operator--() {_current = _current->prev; return *this;}

    Iterator operator--(int) {
      Iterator tmp = *this;
      --(*this);
      return tmp;
    }

    friend bool operator== (const Iterator& a, const Iterator& b) { return a._current == b._current; };
    friend bool operator!= (const Iterator& a, const Iterator& b) { return a._current != b._current; };     
  };

  Iterator begin() const { return Iterator(this->head); }
  Iterator end() const { return Iterator((Node*)nullptr); }

  function<bool(const Node&)> matches_word(const string& target) const {
    return [target](const Node &node) { return node.word == target; };
  }

  auto find_word(const string& target) {
    return find_if(this->begin(), this->end(), matches_word(target));}

   public:
    
    Wordlist() {
      head = nullptr;
      tail = nullptr;
      frozen = (size = 0);
    }

    Wordlist(const Wordlist& source) : Wordlist() {
      for_each(source.begin(), source.end(),
               [this](const Node& node) { this->append_word(node.word); });
      this->frozen = source.frozen;
    }

    Wordlist(const string& file_name) : Wordlist() {
      string temp;
      ifstream text_file(file_name);
      if (text_file.is_open()) {
      while (text_file >> temp) {
        this->add_word(temp);
      }
      }
      text_file.close();
    }

    ~Wordlist() {
      Node* cursor = head;
      while (cursor) {
        Node* tmp = cursor->next;
        delete cursor;
        cursor = tmp;
      }
    }

    bool is_empty() const { return this->length() == 0; }

    int length() const { return size; }

    bool is_frozen() const { return frozen; }

    bool contains(const string& word) const {
      Node* curr = head;
      while (curr && curr->word != word) {
        curr = curr->next;
      }
      return curr ? 1 : 0;
    }

    bool at_end(Node * node) { return node == tail; }

    bool same_word(Node * curr, string const& word) {
      return curr->word == word;
    }

    Node* make_node(string const& word) {
      Node* new_word = new Node;
      new_word->word = word;
      new_word->prev = nullptr;
      new_word->next = nullptr;
      return new_word;
    }

    /**
     * @brief Takes a node and returns true if it is the tail item in the list
     * and it contains the same word as `word`, false otherwise
     *
     * @param node a Node*
     * @param word a const string referencr
     * @return true if the node passed is the  onetail in the list and the node
     * contains the word being searched for
     * @return false if condition above not satisfied
     */
    bool last_word_matches(Node * node, string const& word) {
      return at_end(node) && same_word(node, word);
    }

    /**
     * @brief Takes a word and adds it to the list if the word is not within.
     * Otherwise, does nothing
     *
     * @param word a const string reference
     */
    void add_word(const string& word) {
      assert(!is_frozen());

      if (is_empty()) {
        Node* node = make_node(word);
        head = node;
        tail = node;
        size += 1;
        return;
      }
      Node* curr = head;
      while (!at_end(curr) && !same_word(curr, word)) {
        curr = curr->next;
      }
      if (same_word(curr, word)) {
        return;
      } else {
        assert(at_end(curr));
        Node* new_word = make_node(word);
        new_word->prev = tail;
        tail->next = new_word;
        tail = new_word;
        size += 1;
      }
    }

    /**
     * @brief Takes a word and removes it from the list if it is within.
     * Otherwise, does nothing
     *
     * @param word a string
     */
    void remove_word(const string& word) {
      assert(!is_frozen());
      if (!this->contains(word)) {
        return;
      }
      auto remove = this->find_word(word);
      if (this->length() == 1) {
        head = nullptr;
        tail = nullptr;
      } else if (remove == this->head) {
        head = head->next;
        head->prev = nullptr;
      } else if (remove == this->tail) {
        tail = tail->prev;
        tail->next = nullptr;
      } else {
        remove->prev->next = remove->next;
        remove->next->prev = remove->prev;
      }

      size--;
      delete remove._current;
      return;
    }


/**
 * @brief Takes an index and returns the word at that index if the index is within bounds
 * Otherwise, returns a error
 * @param index an int representing the position of the word
 * @return string the word at the node index refers to
 */
    string get_word(int index) const {
      assert(index > -1 && index < this->size);

      int pos = index;
      Node* curr = head;
      while (pos) {
        curr = curr->next;
        pos--;
      }
      return curr->word;
    }

    vector<string> as_vector() const {
      vector<string> words;
      transform(this->begin(), this->end(), back_inserter(words),
                [](Node& nd) { return nd.word; });
      return words;     
    }

    /**
     * @brief Takes a node of the list and returns the word on the next node
     *
     * @param nd the current node
     * @return the word on the next node
     */
    string next_word(const Node* nd) const { return nd->next->word; }

    /**
     * @brief Takes a node of the list and returns the word on the previous node
     *
     * @param nd the current node
     * @return the word on the previous node
     */
    string prev_word(const Node* nd) const { return nd->prev->word; }

    vector<string*> get_sorted_index() {
      vector<string*> words;
      transform(this->begin(), this->end(), back_inserter(words),
                [](Node& nd) { return &nd.word; });
      sort(words.begin(), words.end(),
           [](string* a, string* b) { return *a < *b; });
      frozen = 1;
      return words;
    }


    // bool tail() {
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
