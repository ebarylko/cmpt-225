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
#include "Wordlist_base.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Wordlist : public Wordlist_base
{
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
// Node* head;
// Node* tail;
bool frozen;

public: 
Wordlist() {
// Node* head = new Node;
// Node* tail = new Node;
// head = nullptr;
// tail = nullptr;
frozen = size = 0;
}

int length() const {
  return size;
}


/* const string get_word(int pos) {
Node* start = head;
while (pos != 0) {
head++;
pos--;
}
return start->word;
}
*/

bool is_frozen() const {
  return frozen;
}

// bool contains(const string& word) const {
// for (Node* curr = head; curr < head + size; curr++) {
// if (curr->word == word) {
//   return true;
// }
// }
// return false;
// } 

// bool first() {
//   return !head;
// }

// bool last() {
//   return !tail;
// }

    //
    // ... your code goes here ...
    //

    //
    // ... you can write helper methods if you need them ...
    //

}; // class Wordlist

//
// ... you can write helper functions here (or before Wordlist) if you need them
// ...
//
