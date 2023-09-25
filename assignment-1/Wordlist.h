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
#include <cassert>
#include <algorithm>
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
   * @brief Takes a word and appends it to the end of the list
   *
   * @param word a string which is appended to the end of the list
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

  // I decided to make an iterator in order to use the 
  // functions in the STL such as transform, find_if, and 
  // for_each. 
  // I used https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
  // to see how to make an iterator
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

    friend bool operator==(const Iterator& a, const Iterator& b) {
      return a._current == b._current;
    };
    friend bool operator!=(const Iterator& a, const Iterator& b) {
      return a._current != b._current;
    };
  };

  Iterator begin() const { return Iterator(this->head); }
  Iterator end() const { return Iterator((Node*)nullptr); }

  /**
   * @brief Takes a word and returns the node that contains the word.
   * 
   * @param target the word to be searched for
   * @return auto the node which has the word
   */
  auto find_word(const string& target) const {
    auto matches_target = [target](const Node& node) {
      return node.word == target;
    };
    return find_if(this->begin(), this->end(), 
    matches_target);};

    /**
     * @brief Takes a word W and returns a node which has the word W
     * 
     * @param word a string
     * @return Node* a node with the word W
     */
    Node* make_node(string const& word) {
      return new Node{word, nullptr, nullptr};
    }

   public:
    
    Wordlist() {
      head = nullptr;
      tail = nullptr;
      frozen = (size = 0);
    }

    Wordlist(const Wordlist& source) : Wordlist() {
      // Copying each node from source over to the current Wordlist
      for_each(source.begin(), source.end(),
               [this](const Node& node) { this->append_word(node.word); });
      this->frozen = source.frozen;
    }

    Wordlist(const string& file_name) : Wordlist() {
      string temp;
      ifstream text_file(file_name);

      if (text_file.is_open()) {
      // add words to the list while there are words in the file
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

    /**
     * @brief Returns true is the list is empty. False otherwise
     * 
     * @return true if the list has no words
     * @return false if the list has 1 or more words
     */
    bool is_empty() const { return this->length() == 0; }

    /**
     * @brief Returns the size of the list
     * 
     * @return int The size of the list
     */
    int length() const { return size; }

    /**
     * @brief Returns true if the list is frozen. False otherwise
     * 
     * @return true if the list is frozen
     * @return false if the condition above is false
     */
    bool is_frozen() const { return frozen; }

    /**
     * @brief Takes a word and returns true if the word is withn the list. 
     * False otherwise
     * 
     * @param word a string representing the word being searched for
     * @return true if the word is in the list
     * @return false if the word is not in the list
     */
    bool contains(const string& word) const {
      return this->find_word(word) != this->end();
    }


    /**
     * @brief If the list is not frozen, it takes a word and adds it 
     * to the list if the word is not within. Otherwise, does nothing
     *
     * @param word a const string reference
     */
    void add_word(const string& word) {
      if (this->is_frozen()) {
        throw runtime_error("You are adding a word onto a frozen list");
      }

      if (this->is_empty()) {
        Node* node = make_node(word);
        head = node;
        tail = node;
        size += 1;
        return;
      }

      if (this->contains(word)) {
        return;
      } 

        Node* new_word = make_node(word);
        new_word->prev = tail;
        tail->next = new_word;
        tail = new_word;
        size += 1;
    }

    /**
     * @brief Throws an error if the list is frozen.
     * If the list is not frozen, the word passed is removed from the list 
     * if it is within. Otherwise, nothing happens.
     *
     * @param word a string representing the word to be removed
     */
    void remove_word(const string& word) {
      if (this->is_frozen()) {
        throw runtime_error("You are removing a word from a frozen list");
      }

      auto remove = this->find_word(word);
      if (remove == this->end()) {
        return;
      }

      // Checking if the list will be empty after removing word
      if (this->length() == 1) {
        head = nullptr;
        tail = nullptr;

      // Checking if the removed node is the first item
      } else if (remove == this->head) {
        head = head->next;
        head->prev = nullptr;
      
      // Checking if the removed node is the last item
      } else if (remove == this->tail) {
        tail = tail->prev;
        tail->next = nullptr;
      } else
      // What happens when the word is inbetween the head and tail
      {
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
      // Check that the index passed is valid
      assert(index >= 0 && index < this->size);

      Iterator list(this->begin());
      // Iterate over the list until we are at the desired position
      advance(list, index);
      return list->word;
    }

    // the following method was used for testing 
    /**
     * @brief Returns a vector of all the words in the list
     *
     * @return vector<string> a vector containing all the words in the list
     */
    vector<string> as_vector() const {
      vector<string> words;
      transform(this->begin(), this->end(), back_inserter(words),
                [](Node& nd) { return nd.word; });
      return words;     
    }

    /**
     * @brief Returns a vector of string pointers in the list ordered alphabetically
     * 
     * @return vector<string*> A vector of string pointers ordered alphabetically
     */
    vector<string*> get_sorted_index() {
      vector<string*> words;

      // I use transform so I can create a vector of string*
      transform(this->begin(), this->end(), back_inserter(words),
                [](Node& nd) { return &nd.word; });

      // Sorting all the words alphabetically
      sort(words.begin(), words.end(),
           [](string* a, string* b) { return *a < *b; });

      frozen = 1;
      return words;
    }
  };  // class Wordlist

