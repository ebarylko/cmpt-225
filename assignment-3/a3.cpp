// a3.cpp

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
// Do not #include any other files!
//
#include "Announcement.h"
#include "JingleNet_announcer.h"
#include "Queue_base.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;


// Dos tipos de insrucciones. 
// una que modifica la lista entera
// y la otra que solo modifica un queue

template <typename T> class Queue : public Queue_base<T> {
    
    // typedef Body string;
    // typedef Sender string;
    // typedef Reciever string;
    // struct Message {
    //    Sender sender;
    //    Receiver recipient;
    //    Body content; 
    //    Message* next_message;
    //    Messsage* prev_message;
    // }


// message has a current, last, and next message for the queue

    struct Node {
       Node* prev;
       T curr;
       Node* next;
    };

    Node* first;
    Node* last;
    int elems = 0;

    public :

    int size() {
        return this->elems;
    }

    bool is_empty() {
        return !this->size();
    }

    void enqueue(const T& item) {
       Node* new_item = new Node(nullptr, item, nullptr);
       if (this->empty()) {
         first = new_item;
         last = new_item;
         elems++;
       } else {
         this->last->next = new_item;
         new_item->prev = this->last;
         this->last = new_item;
       }
    }


};      

// JingleNet has 
// a queue for each of the following:
// santa, reindeer, elf2, elf1, snowman
// the following methods: send, announce,
// promote, and remove_all
// Idea: maybe put all the queues in a collection so
// you can always acceess a specific queue quickly
class JingleNet {

};

int main()
{
    cout << "Welcome to Assignment 3!" << endl;
}
