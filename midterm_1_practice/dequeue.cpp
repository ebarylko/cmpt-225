#include <iostream>
#include "dequeue.h"
using namespace std;

// The purpose of this file is to create the interface for the
// dequeue class. It will contain the standard operations 
// and the implementation.


Dequeue::Dequeue(): _size(0), _head(nullptr), _tail(nullptr) {};

Dequeue::~Dequeue() {
    Node* removing = this->_head;
    Node* next;

    while (removing) {
        next = removing->next;
        delete removing;
        removing = next;
    }
}

/**
 * @brief Returns the value of the first item in the deck if the dequeue is 
 * not empty
 * 
 * @return int 
 */
int Dequeue::first() {
    assert(this->_head);
    return this->_head->val;
}


int Dequeue::size() {
    return _size;
}

bool Dequeue::is_empty() {
    return !this->size();
}

/**
 * @brief Takes an element and adds it to the front of the list
 * 
 * @param val 
 */
void Dequeue::push_front(int val) {
    Node* new_node = new Node(val, nullptr, this->_head);

    if (this->_head) {
        this->_head->prev = new_node;
    } else {
        this->_tail = new_node;
    }

    this->_head = new_node;
}


void Dequeue::push_back(int val) {
    Node* new_node = new Node(val, this->_tail, nullptr);

    if (this->_tail) {
        this->_tail->next = new_node;
    } else {
        this->_head = new_node;
    }

    this->_tail = new_node;
}

/**
 * @brief Removes the element of the front of the dequeue
 * 
 */
void Dequeue::rest() {
    assert(!this->is_empty());
    Node* remove = this->_head;
    this->_head = this->_head->next;
    this->_size--;

    if (this->is_empty()) {
        this->_tail = nullptr;
    } else {
        this->_head->prev = nullptr;
    }

    delete remove;
}

/**
 * @brief Removes the element at the end of the dequeue if 
 * it is not empty.
 * 
 */
void Dequeue::remove_last() {
    assert(!this->is_empty());
    Node* remove = this->_tail;
    this->_tail = this->_tail->prev;
    this->_size--;

    if (this->is_empty()) {
        this->_head = nullptr;
    } else {
        this->_tail->next = nullptr;
    }

    delete remove;
}

int main() {
    return 0;
}