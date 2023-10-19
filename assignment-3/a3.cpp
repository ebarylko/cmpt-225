#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
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

using namespace std;


struct InProgress {
    string found;
    string rest;
};    

InProgress read_word(string search_in) {
    int space_pos = search_in.find_first_of(" ");
    InProgress res;
    
    if (space_pos != -1) {
    res.found = search_in.substr(0, space_pos);
    res.rest = search_in.substr(space_pos + 1);
    }

    return res;
}

template <typename T>
class Queue : public Queue_base<T> {
  struct Node {
    Node* prev;
    T curr;
    Node* next;

    Node(T curr) :  prev(nullptr), curr(curr), next(nullptr){};
  };

  Node* first;
  Node* last;
  int elems;

 public:
  Queue<T>() : first(nullptr), last(nullptr), elems(0){};
  ~Queue<T>() {
    while (!this->is_empty()) {
      this->dequeue();
    }
  };

  int size() const { return this->elems; }

  bool is_empty() const {
    return !this->size();
  }

  void enqueue(const T& item) {
    Node* new_item = new Node(item);
    if (this->is_empty()) {
      this->first = new_item;
      this->last = new_item;
    } else {
      this->last->next = new_item;
      new_item->prev = this->last;
      this->last = new_item;
    }
    this->elems++;
  }

  void dequeue() {
    if (this->is_empty()) {
      throw runtime_error("dequeue: queue is empty");
    }
    Node* remove = this->first;
    this->first = this->first->next;
    this->elems--;
    this->last = this->is_empty() ? nullptr : this->last;
    delete remove;
  }

  vector<T> print_items() {
    vector<T> itms;
    Node* curr_itm = this->first;
    while (curr_itm) {
      itms.push_back(curr_itm->curr);
      curr_itm = curr_itm->next;
    }
    return itms;
  };

  const T& front() const {
    if (this->is_empty()) {
      throw runtime_error("front: queue is empty");
    }
    return this->first->curr;
  }
};

struct Message {
  typedef string Body, Sender, Receiver;
  Sender sender;
  Body content;
  Message(const Sender& from,const Body& content): sender(from), content(content){};
};

ostream& operator<<(ostream& os, const Message& msg) {
  os << msg.sender << ' ';
  os << msg.content;
  return os;
}

bool operator==(const Message& one, const Message two) {
    return one.content == two.content && one.sender == two.sender;
}

enum Receiver { Santa, Reindeer, Elf2, Elf1, Snowman };
// preguntar professor sobre testing

// JingleNet has
// a queue for each of the following:
// santa, reindeer, elf2, elf1, snowman
class JingleNet {

  // Base class for an instruction which contains a method for executing a
  // specific instruction
  class Instruction {
   public:
    virtual void execute(JingleNet system) = 0;
    virtual ~Instruction(){};
  };

    Queue<Message>& get_queue(const Rank& to) {
        return messages[(int)to - 1];
    }
  // Enqueues the message from the sender to the queue where the receiver is

     void send_message(const Message& msg, Rank to) {
        get_queue(to).enqueue(msg);
    }

  Queue<Message> messages[5];


 public:
  /**
   * @brief Takes an instruction and applies the instruction onto the JingleNet
   * system
   *
   * @param instruction One of four possible commands to be executed by
   * JingleNet
   */
  void apply_instruction(const string& instruction) {
    string message;
    InProgress instr = read_word(instruction);
    string send_instr = "SEND";
    // Send message if the instruction was send
    if (send_instr == instr.found) {
      InProgress sender = read_word(instr.rest);
      InProgress receiver = read_word(sender.rest);
      Message msg(sender.found, receiver.rest);
      Rank to = to_rank(receiver.found);
      this->send_message(msg, to);
    }
  }

  /**
   * @brief Takes a receiver and returns all the messages for that person
   *
   * @param receiver one of the following targets: santa, reindeer, elf2, elf1,
   * snowman
   * @return string all the messages the target has received
   */
  vector<Message> const messages_for(const Rank& receiver) {
    return get_queue(receiver).print_items();
  }
};

// parsiar una instruccion:
// ir linea por linea, y crear commandos
// applicar commandos sobre las lineas

// hacer una clase que te dice que estas testiando

void test_case(string scenario) { cout << scenario << endl; }

// vector<Message> mk_msgs(initializer_list<Message>) {

// }
void print_message(Message msg) {
    cout << msg << endl;
}

void print_messages(vector<Message> messages) {
    for_each(messages.begin(), messages.end(), print_message );
}

void send_test() {

{ 
  JingleNet system;
  test_case(
      "Adding a message for santa creates a new message in the santa queue");
  string instruction = "SEND a santa hi";
  system.apply_instruction(instruction);
  vector<Message> expected{Message("a", "hi")};
  Rank target = Rank::SANTA;
  vector<Message> actual = system.messages_for(target);
  assert(expected == actual);
 }

 {
  JingleNet sys;
  test_case(
      "Adding two messages for santa creates a queue with two messages that "
      "has "
      "the oldest message first");
  string instr_1 = "SEND a santa 1";
  string instr_2 = "SEND a santa 2";

  sys.apply_instruction(instr_1);
  sys.apply_instruction(instr_2);
  vector<Message> expected{Message("a", "1"), Message("a", "2")};
  Rank target = Rank::SANTA;
  vector<Message> actual = sys.messages_for(target);
  assert(expected == actual);
 }
}


// int main() {
//   cout << "Welcome to Assignment 3!" << endl;
// }

namespace doctest {
template <typename T>
struct StringMaker<std:: vector<T>>
{
    static String convert(const std::vector<T>& in) {
        std::ostringstream oss;

        oss << "[";
        // NOLINTNEXTLINE(*-use-auto)
        for (typename std::vector<T>::const_iterator it = in.begin(); it != in.end();) {
            oss << *it;
            if (++it != in.end()) { oss << ", "; }
        }
        oss << "]";
        return oss.str().c_str();
    }
};
}  

TEST_CASE("Testing all the methods of JingleNet"){
    SUBCASE("Testing send"){
        SUBCASE("Adding a single message onto a queue should result in a queue with one message matching the one added") {
        GIVEN("An empty queue") {
        JingleNet system;
        WHEN("Adding a message for santa") {
        string instruction = "SEND a santa hi";
        system.apply_instruction(instruction);
        THEN("The santa queue should have one message matching the message added before") {
        vector<Message> expected{Message("a", "hi")};
        Rank target = Rank::SANTA;
        vector<Message> actual = system.messages_for(target);
        REQUIRE(expected == actual);
        }
        }
        }
        }
    SUBCASE(
        "Adding two messages for santa creates a queue with two messages that "
        "has the oldest message first") {
        GIVEN("An empty queue") {
        JingleNet sys;
        WHEN("Adding two messages for santa") {
        string instr_1 = "SEND a santa 1";
        string instr_2 = "SEND a santa 2";
        sys.apply_instruction(instr_1);
        sys.apply_instruction(instr_2);
        THEN("There should be two messages in the santa queue, with the oldest being first in the queue") {
        vector<Message> expected{Message("a", "1"), Message("a", "2")};
        Rank target = Rank::SANTA;
        vector<Message> actual = sys.messages_for(target);
        REQUIRE(expected == actual);
        }
        }
        }
    };
    }
    // SUBCASE("testing announce") {

    // }

}