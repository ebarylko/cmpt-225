#pragma once
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Announcement.h"
#include "JingleNet_announcer.h"
#include "Queue_base.h"


template <typename T>
class Queue : public Queue_base<T> {
  struct Node {
    Node* prev;
    T curr;
    Node* next;

    Node(T curr);
  };

  Node* first;
  Node* last;
  int elems;

 public:
  Queue<T>();
  ~Queue<T>();

  int size() const; 

  bool is_empty() const; 

  void enqueue(const T& item); 

  void dequeue(); 

  vector<T> print_items(); 

  const T& front() const; 
};

struct Message {
  typedef string Body, Sender, Receiver;
  Sender sender;
  Body content;
  Message(const Sender& from,const Body& content);
};

class JingleNet {

  // Base class for an instruction which contains a method for executing a
  // specific instruction
  class Instruction {
   public:
    virtual void execute(JingleNet system) = 0;
    virtual ~Instruction(){};
  };

    Queue<Message>& get_messages(const Rank& to);
  // Enqueues the message from the sender to the queue where the receiver is

     void send_message(const Message& msg, Rank to); 

  Queue<Message> messages[5];

 public:
  void apply_instruction(const string& instruction); 

  vector<Message> const messages_for(const Rank& receiver); 
};