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
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

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

#include <vector>
template <typename T>
class Queue : public Queue_base<T> {
  struct Node {
    Node* prev;
    T curr;
    Node* next;

    Node(T curr) :  prev(nullptr), curr(curr), next(nullptr){};
    ~Node() {};
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

  bool has_items() const {
     return this->size(); 
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
    this->first = remove->next;
    this->elems--;
    if (this->has_items()) {
        this->first->prev = nullptr;
    }

    this->last = this->is_empty() ? nullptr : this->last;
    delete remove;
  }


  const T& front() const {
    if (this->is_empty()) {
      throw runtime_error("front: queue is empty");
    }
    return this->first->curr;
  }

// remove the following
  vector<T> items();
};

struct Message {
  typedef string Body, Sender, Receiver;
  Sender sender;
  Body content;
  Message(): sender(""), content("") {};
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

  // Enqueues the message from the sender to the queue where the receiver is

     void send_message(const Message& msg, Rank to) {
        get_messages(to).enqueue(msg);
    }

  Queue<Message> messages[5];
  /**
   * @brief Takes a number of messages to remove N and a receiver (santa|reindeer|elf2|elf1|snowman)
   *  and removes N messages from the receiver's queue (less if the queue has less than N messages).
   *  Returns the amount of messages left to remove in the other queues
   *
   * @param msgs_to_announce the number of messages to announce
   * @param target the receiver to remove messages from
   * @return int the amount of messages to remove in the other queues
   */
  // tal vez puedo hacer esto mas efficiente si
  // no hago nada cuandoo no tengo un mensaje para
  // annunciar
  int remove_msgs(int msgs_to_announce, const Rank target) {
        Message announcing;
        Queue<Message>& to_remove = this->get_messages(target);
        // Announcing all the messages and removing them from
        // the queue
        while (to_remove.has_items() && msgs_to_announce != 0) {
            announcing = to_remove.front();
            Announcement a(announcing.sender, target, announcing.content);
            jnet.announce(a);
            to_remove.dequeue();
            msgs_to_announce--;
        }

        return msgs_to_announce;
  }


Rank receiver[5] = {Rank::SANTA, Rank::REINDEER, Rank::ELF2, Rank::ELF1, Rank::SNOWMAN};
/**
 * @brief takes a number of messages to announce N and announces N messages in total from the queue,
 * starting from the santa queue and then removing more messages from the rest of the queues
 * Ex: if given five messages to announce and there are only three messages for santa and the reindeers,
 * [[santa: [msg1, msg2, msg3]
 * [reindeer: [msg4, msg5, msg6]]
 * applying the method would return the following queue;
 * [[santa: []
 * [reindeer: [msg6]]]
 * 
 * @param num the number of messaages to announce
 */
void announce_msgs(int num) {
    // remover itemas del primer queue, despues continuar con los otros queues si es necessario.
    // int msgs_left = this->remove_msgs(num, Rank::SANTA);
    Rank target;
    for (int pos = 0; pos < 5 && num != 0; pos++) {
            target = receiver[pos];
            num = this->remove_msgs(num, target);
    }
}

 public:
  ~JingleNet(){};

  Queue<Message>& get_messages(const Rank& to) {
        return messages[(int)to - 1];
  }


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
    string announce_instr = "ANNOUNCE";
    // Send message if the instruction was send
    if (send_instr == instr.found) {
      InProgress sender = read_word(instr.rest);
      InProgress receiver = read_word(sender.rest);
      Message msg(sender.found, receiver.rest);
      Rank to = to_rank(receiver.found);
      this->send_message(msg, to);
    // announce message if instruction was to announce
    } else if (announce_instr == instr.found) {
        int announce_num = stoi(instr.rest);
        this->announce_msgs(announce_num);
    }
  }





// void print_message(Message msg) {
//     cout << msg << endl;
// }

// void print_messages(vector<Message> messages) {
//     for_each(messages.begin(), messages.end(), print_message );
// }


// int main() {
//   cout << "Welcome to Assignment 3!" << endl;
// }


};


#include <map>
#include <sstream>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"


// definir iterador para un queue para que puedas crear una funcion en lugar de un metodo
template <typename T> vector<T> Queue<T>::items() {
  vector<T> itms;
  Node* curr_itm = this->first;
  while (curr_itm) {
    itms.push_back(curr_itm->curr);
    curr_itm = curr_itm->next;
  }
  return itms;
};

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


  /**
   * @brief Takes a receiver and returns all the messages for that person
   *
   * @param receiver one of the following targets: santa, reindeer, elf2, elf1,
   * snowman
   * @return string all the messages the target has received
   */
  const vector<Message> messages_for(JingleNet& j, const Rank& receiver) {
    return j.get_messages(receiver).items();
  }

typedef map<Rank, vector<Message>> AllMessages;
vector<Rank> receivers = {Rank::SANTA, Rank::REINDEER, Rank::ELF2, Rank::ELF1, Rank::SNOWMAN};
AllMessages all_messages(JingleNet& j) {
    AllMessages all;
    for_each(receivers.begin(), receivers.end(), [&all, &j](const Rank target) {all[target] = messages_for(j, target);});
    return all;
}

bool empty_msg(pair<Rank, vector<Message>> rnk_and_msg) {
    return rnk_and_msg.second.empty();
}

bool has_no_messages(AllMessages& messages) {
    return all_of(messages.begin(), messages.end(), empty_msg);
}


  TEST_CASE("JingleNet") {
    SUBCASE("send") {
        SUBCASE("An empty JingleNet has not messages") {
            GIVEN("An empty JingleNet") {
              JingleNet system;
              THEN("All queues are empty") {
                AllMessages msgs = all_messages(system);
                REQUIRE(has_no_messages(msgs));
              }
            }
        }
        SUBCASE("Sending a message adds it to the queue ") {
            GIVEN("An empty JingleNet") {
              JingleNet system;
              WHEN("Sending a message to santa") {
                string instruction = "SEND a santa hi";
                system.apply_instruction(instruction);
                THEN("The santa queue has the message") {
                  vector<Message> expected{Message("a", "hi")};
                  Rank target = Rank::SANTA;
                  vector<Message> actual = messages_for(system, target);
                  REQUIRE(expected == actual);
                }
              }
            }
        }
        SUBCASE("Sending two messages to santa"){
            GIVEN("An empty JingleNet"){JingleNet sys;
        WHEN("Sending two messages to santa") {
            string instr_1 = "SEND a santa 1";
            string instr_2 = "SEND a santa 2";
            sys.apply_instruction(instr_1);
            sys.apply_instruction(instr_2);
            THEN("The santa queue has the two messages") {
              vector<Message> expected{Message("a", "1"), Message("a", "2")};
              Rank target = Rank::SANTA;
              vector<Message> actual = messages_for(sys, target);
              REQUIRE(expected == actual);
            }
        }
    }
  };
  }
  SUBCASE("announce") {
      SUBCASE("Announcing messages when there are none has no effect") {
          GIVEN("An empty JingleNet") {
              JingleNet sys;
              WHEN("A message is announced") {
                  string announce_msg = "ANNOUNCE 1";
                  sys.apply_instruction(announce_msg);
                  THEN("The queues remained unchanged") {
                     AllMessages msgs = all_messages(sys);
                     REQUIRE(has_no_messages(msgs));
                  }
              }
          }
      }
      SUBCASE("Announcing a single message will print it to announcements.txt") {
        GIVEN("A empty JingleNet") {
              JingleNet sys;
              WHEN("A message is added and announced") {
                string instr_send = "SEND a santa 1";
                string instr_announce = "ANNOUNCE 1";
                sys.apply_instruction(instr_send);
                sys.apply_instruction(instr_announce);
                THEN("There will be a messsage in announcements.txt and the santa queue will be empty") {
                     AllMessages msgs = all_messages(sys);
                     REQUIRE(has_no_messages(msgs));

                }
              }
        }
      }
  }
  }