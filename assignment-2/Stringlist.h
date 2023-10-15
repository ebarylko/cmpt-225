// Stringlist.h

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
// Do not use any other #includes or #pragmas in this file.
//

#pragma once

#include <cassert>
#include <iostream>
#include <string>

using namespace std;

class Stringlist {
  int cap;      // capacity
  string *arr;  // array of strings
  int sz;       // size

  /**
   * @brief Takes a list, and resets it to a default list
   *
   * @param list the list to reset
   */
  void reset() {
    this->cap = 10;
    this->sz = 0;
    this->arr = new string[this->cap];
  }

  // The following three functions are the implementation for the
  // set, remove_at, and insert_before functions, respectively. They have been
  // moved from the original functions in order to
  // seperate the act of doing an undoable action from
  // the act of doing the action itself.

  /**
   * @brief Takes a position P in the list and a word W, and changes
   * the word at P to match W
   *
   */
  void set_impl(int index, string word) { arr[index] = word; }

  /**
   * @brief Remove at index implementation without undo nor bounds check
   * @pre index is 0 <= index < sz
   * @post sz = sz@pre - 1 && arr[index..sz-1] = arr@pre[index+1 .. sz@pre -
   * 1]
   * @param index Index to remove
   */
  void remove_at_impl(int index) {
    for (int i = index; i < sz - 1; i++) {
      arr[i] = arr[i + 1];
    }
    sz--;
  }

  /**
   * @brief Takes a position in the list and a word, and inserts the word at
   * the position if the position is valid. Otherwise, an error is thrown.
   *
   * @param index a position within or outside the list
   * @param s the word to insert at index
   */
  void insert_before_imp(int index, const string &s) {
    if (index < 0 || index > sz) {  // allows insert at end, i == sz
      bounds_error("insert_before");
    }
    check_capacity();

    for (int i = sz; i > index; i--) {
      arr[i] = arr[i - 1];
    }
    arr[index] = s;
    sz++;
  }

  // Base class for all undoable actions
  class UndoAction {
   public:
    virtual ~UndoAction(){};
    virtual void undo(Stringlist &) = 0;
  };

  // the UndoStack contains a collection of undoable actions, with the most
  // recent actions appearing earlier in the list. The undo method of the list
  // pops the most recent action off the stack and applies it to the list,
  // returning the list to how to was before the method which spawned the undo 
  // action was called.
  // An example is if the user inserts a word at the beginning of a new list,
  // resulting in a list like ["1"]. The undo stack would then contain the 
  // corresponding action which undoes this change, and calling undo would
  // apply the action onto the list and return an empty list.
  // The undo stack would be empty after this

  struct UndoStack {
    struct Node {
      UndoAction *action;
      Node *next;

      ~Node() { delete this->action; }
    };

    Node *head;
    int size;

    UndoStack() : head(nullptr), size(0){};

    ~UndoStack() {
      Node *curr = head;
      Node *nxt;

      while (curr) {
        nxt = curr->next;
        delete curr;
        curr = nxt;
      }
    }

    /**
     * @brief Takes an undoable action and pushes it onto the stack
     *
     * @param e The newest undoable action
     */
    void push(UndoAction* e) {
      Node *nd = new Node;
      nd->action = e;
      nd->next = this->head;

      this->head = nd;
      this->size++;
    }

    /**
     * @brief Returns the first undoable action from the stack if the stack
     * is not empty.
     *
     * @return Node* An undoable action
     */
    Node *pop() {
      assert(this->size);

      Node *first = this->head;
      this->head = this->head->next;
      this->size--;
      return first;
    }

    /**
     * @brief Takes a list and applies the latest undoable action to it
     *
     * @param lst the list being passed
     */
    void undo(Stringlist &lst) {
      Node *tmp = this->pop();
      tmp->action->undo(lst);
      delete tmp;
    }
  } st;

  // The undoable action for insert_before that
  // when applied to the list removes the word at
  // the position used by the insert_before method
  // which created it.
  struct UndoInsertBefore : UndoAction {
    int target;

    UndoInsertBefore(int index) : target(index){};

    void undo(Stringlist &instance) { instance.remove_at_impl(this->target); }
  };

  // The undoable action for remove_at that
  // when applied to the list inserts the word
  // that remove_at erased in its original position
  struct UndoRemoveAt : UndoAction {
    int target;
    string word;

    UndoRemoveAt(const int index, const string word)
        : target(index), word(word){};

    void undo(Stringlist &instance) {
      instance.insert_before_imp(target, word);
    }
  };

  // The undoable action for remove_all and the assignment operator
  // which when applied to the list returns it to how it was before
  // either operation was called
  struct UndoChangeToWholeList : UndoAction {
    string *words;
    int cap, size;

    UndoChangeToWholeList(Stringlist &instance)
        : words(instance.arr), cap(instance.cap), size(instance.sz){};

    ~UndoChangeToWholeList() { delete[] words; }

    // Change the current list to match the list stored here
    void undo(Stringlist &instance) {
      delete[] instance.arr;
      instance.arr = words;
      instance.cap = cap;
      instance.sz = size;
      words = nullptr;
    }
  };

  // The undoable action for set that
  // when called on the list reverts the word 
  // at the specific position back to its value before set was called
  struct UndoSet : UndoAction {
    int target;
    string word;

    UndoSet(const int index, const string word) : target(index), word(word){};

    void undo(Stringlist &instance) { instance.set_impl(target, word); }
  };

  //
  // Helper function for throwing out_of_range exceptions.
  //
  void bounds_error(const string &s) const {
    throw out_of_range("Stringlist::" + s + " index out of bounds");
  }

  //
  // Helper function for checking index bounds.
  //
  void check_bounds(const string &s, int i) const {
    if (i < 0 || i >= sz) {
      bounds_error(s);
    }
  }

  //
  // Helper function for copying another array of strings.
  //
  void copy(const string *other) {
    for (int i = 0; i < sz; i++) {
      arr[i] = other[i];
    }
  }

  //
  // Helper function for checking capacity; doubles size of the underlying
  // array if necessary.
  //
  void check_capacity() {
    if (sz == cap) {
      cap *= 2;
      string *temp = new string[cap];
      for (int i = 0; i < sz; i++) {
        temp[i] = arr[i];
      }
      delete[] arr;
      arr = temp;
    }
  }

 public:
  //
  // Default constructor: makes an empty StringList.
  //
  Stringlist() { this->reset(); }

  //
  // Copy constructor: makes a copy of the given StringList.
  //
  // Does *not* copy the undo stack, or any undo information from other.
  //
  Stringlist(const Stringlist &other)
      : cap(other.cap), arr(new string[cap]), sz(other.sz) {
    copy(other.arr);
  }

  //
  // destructor
  //
  ~Stringlist() { delete[] arr; }

  //
  // Assignment operator: makes a copy of the given StringList.
  //
  // undoable
  //
  // For undoing, when assigning different lists, the undo stack is NOT
  // copied:
  //
  //    lst1 = lst2; // lst1 undo stack is updated to be able to undo the
  //                 // assignment; lst1 does not copy lst2's stack
  //                 //
  //                 // lst2 is not change in any way
  //
  // Self-assignment is when you assign a list to itself:
  //
  //    lst1 = lst1;
  //
  // In this case, nothing happens to lst1. Both its string data and undo
  // stack are left as-is.
  //
  Stringlist &operator=(const Stringlist &other) {
    if (this != &other) {
      this->st.push(new UndoChangeToWholeList(*this));

      // Change current list to match other
      cap = other.capacity();
      arr = new string[cap];
      sz = other.size();
      copy(other.arr);
    }
    return *this;
  }

  //
  // Returns the number of strings in the list.
  //
  int size() const { return sz; }

  //
  // Returns true if the list is empty, false otherwise.
  //
  bool empty() const { return size() == 0; }

  //
  // Returns the capacity of the list, i.e. the size of the underlying array.
  //
  int capacity() const { return cap; }

  //
  // Returns the string at the given index.
  //
  string get(int index) const {
    check_bounds("get", index);
    return arr[index];
  }

  //
  // Returns the index of the first occurrence of s in the list, or -1 if s is
  // not in the lst.
  //
  int index_of(const string &s) const {
    for (int i = 0; i < sz; i++) {
      if (arr[i] == s) {
        return i;
      }
    }
    return -1;
  }

  //
  // Returns true if s is in the list, false otherwise.
  //
  bool contains(const string &s) const { return index_of(s) != -1; }

  //
  // Returns a string representation of the list.
  //
  string to_string() const {
    string result = "{";
    for (int i = 0; i < size(); i++) {
      if (i > 0) {
        result += ", ";
      }
      result += "\"" + get(i) + "\"";
    }
    return result + "}";
  }

  /*
   * @brief Takes a position and a word, and changes the word at that position
   * in the list to match the word passed if the position is valid. Otherwise,
   * an error is thrown
   *
   * @param index the position of a word in the list
   * @param value the word being passed
   */
  void set(int index, string value) {
    check_bounds("set", index);
    UndoAction *undo = new UndoSet(index, arr[index]);
    st.push(undo);
    set_impl(index, value);
  }

  /**
   * @brief Takes a position in the list and a word, and inserts the word at
   * the position if the position is valid. Otherwise, an error is thrown.
   *
   * @param index a position within or outside the list
   * @param s the word to insert at index
   */
  void insert_before(int index, const string &s) {
    insert_before_imp(index, s);
    auto undo = new UndoInsertBefore(index);
    st.push(undo);
  }

  //
  // Appends s to the end of the list; if necessary, the capacity of the
  // underlying array is doubled.
  //
  // undoable
  //
  void insert_back(const string &s) { insert_before(size(), s); }

  //
  // Inserts s at the front of the list; if necessary, the capacity of the
  // underlying array is doubled.
  //
  // undoable
  //
  void insert_front(const string &s) {
    insert_before(0, s);
    auto undo = new UndoInsertBefore(0);
    st.push(undo);
  }

  //
  // Removes the string at the given index; doesn't change the capacity.
  //
  // undoable
  //
  void remove_at(int index) {
    check_bounds("remove_at", index);

    // add undo
    st.push(new UndoRemoveAt(index, arr[index]));
    // call impl
    this->remove_at_impl(index);
  }

  //
  // Removes all strings from the list; doesn't change the capacity.
  //
  // undoable
  //
  void remove_all() {
    st.push(new UndoChangeToWholeList(*this));
    this->reset();
  }

  //
  // Removes the first occurrence of s in the list, and returns true. If s is
  // nowhere in the list, nothing is removed and false is returned.
  //
  // undoable
  //
  bool remove_first(const string &s) {
    int index = index_of(s);
    if (index == -1) {
      return false;
    }

    remove_at(index);
    return true;
  }

  //
  // Undoes the last operation that modified the list. Returns true if a
  // change was undone.
  //
  // If there is nothing to undo, does nothing and returns false.
  //
  bool undo() {
    // if there's anything in the stack
    if (!st.size) {
      return false;
    }

    st.undo(*this);
    return true;
  }

};  // class Stringlist

//
// Prints list to in the format {"a", "b", "c"}.
//
ostream &operator<<(ostream &os, const Stringlist &lst) {
  return os << lst.to_string();
}

//
// Returns true if the two lists are equal, false otherwise.
//
// Does *not* consider any undo information when comparing two Stringlists. All
// that matters is that they have the same strings in the same order.
//
bool operator==(const Stringlist &a, const Stringlist &b) {
  if (a.size() != b.size()) {
    return false;
  }
  for (int i = 0; i < a.size(); i++) {
    if (a.get(i) != b.get(i)) {
      return false;
    }
  }
  return true;
}

//
// Returns true if the two lists are not equal, false otherwise.
//
// Does *not* consider any undo information when comparing two Stringlists.
//
bool operator!=(const Stringlist &a, const Stringlist &b) { return !(a == b); }
