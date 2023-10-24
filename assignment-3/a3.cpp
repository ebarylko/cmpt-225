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

bool has_one_node() {
    return this->elems == 1;
}

bool is_first_node(Node* nd) {
    return nd == this->first;
}

bool is_last_node(Node* nd) {
    return nd == this->last;
}

  /**
   * @brief Takes a node and removes it from the queue
   *
   * @param remove the node to remove
   * @return Node* the next node in the list following the deleted node
   */
  Node* remove_node(Node* remove) {
    if (has_one_node()) {
        this->first = nullptr;
        this->last = nullptr;

    } else if (is_first_node(remove)) {
        this->first = remove->next;
        this->first->prev = nullptr;

    } else if (is_last_node(remove)) {
        this->last = remove->prev;
        this->last->next = nullptr;

    } else {
        remove->prev->next = remove->next;
        remove->next->prev = remove->prev;
    }
    this->elems--;
    delete remove;
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
  int announce_n(int msgs_to_announce, const Rank target) {
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
    Rank target;
    for (int pos = 0; pos < 5 && num != 0; pos++) {
            target = receiver[pos];
            num = this->announce_n(num, target);
    }
}

 public:
  ~JingleNet(){};

  Queue<Message>& get_messages(const Rank& to) {
        return messages[(int)to - 1];
  }

typedef Queue<Message>& QueueEdit;

// Queue<Message>& remove(Queue<Message>, pred func) {
//     return func(queue)
// }

bool same_sender(const Message& msg, const string& sender) {
    return msg.sender == sender;
}

Queue<Message>& move_msgs(Queue<Message>& src, Queue<Message>& dest) {
    while (src.has_items()) {
        dest.enqueue(src.front());
        src.dequeue();
    }
    return dest;
}

Queue<Message>& remove_msgs(Queue<Message>& src, const string& sender) {
    Queue<Message> cpy;
    Message msg_to_check;
    while (src.has_items()) {
        msg_to_check = src.front();
        if (!same_sender(msg_to_check, sender)) {
            cpy.enqueue(msg_to_check);
        }
        src.dequeue();
    }
   return move_msgs(cpy, src);
}

/**
 * @brief Takes a sender name S and removes all messages with S as the sender
 * 
 * @param sender the sender whose messages will be removed
 */
void remove_all(const string& sender) {
    for (int pos = 0; pos < 5; pos++) {
            this->messages[(int)receiver[pos] - 1] = this->remove_msgs(get_messages(receiver[pos]), sender);
    }
}

/**
 * @brief Takes a collection of messages and a sender and moves
 * all the messages with that specific sender to the next highest queue
 * 
 * @param coll the collection to look over
 * @param sender the sender whose messages we will be moving
 */
Queue<Message>& promote_msgs(Queue<Message>& coll, const string& sender, Rank nxt_target) {
    Queue<Message> tmp;
    Message msg;
    while (coll.has_items()) {
        msg = coll.front();
        if (same_sender(msg, sender)) {
            this->get_messages(nxt_target).enqueue(msg);
        } else {
            tmp.enqueue(msg);
        }
        coll.dequeue();
    }

   return move_msgs(tmp, coll);
    
}

Rank num_to_receiver(const int pos) {
    return receiver[pos];
}

/**
 * @brief Takes a sender and moves all the messsages from that sender one 
 * queue higher than they originally were
 * 
 * @param sender the sender of the messages to move
 */
void promote_messages(const string& sender) {
    Rank target;
    for(int pos = 1; pos < 5; pos++) {
        target = num_to_receiver(pos);
        this->messages[pos] = promote_msgs(this->get_messages(target), sender, target);
    }
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
    string remove_all_instr = "REMOVE_ALL";
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
        // Remove all messages containing the sender
    } else if (remove_all_instr == instr.found) {
        this->remove_all(instr.rest);
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
#include <stdarg.h>

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
template <typename T>
struct StringMaker<map<Rank, vector<T>>> 
{
    static String convert(const map<Rank, vector<T>>& in) {
        std::ostringstream oss;

        oss << "[";
        // NOLINTNEXTLINE(*-use-auto)
        // what type does this have?
        for (auto it = in.begin(); it != in.end();) {
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

ostream& operator<<(ostream& os, const vector<Message>& msgs) {
  for_each(msgs.begin(), msgs.end(), [&os](const Message& msg) {
    os << msg;
  });
  return os;
}

ostream& operator<<(ostream& os, const pair<Rank, vector<Message>> trget_and_msgs) {
  os << trget_and_msgs.second;
  return os;
}

typedef map<Rank, vector<Message>> AllMessages;
ostream& operator<<(ostream& os, const AllMessages& msgs) {
  for_each(msgs.begin(), msgs.end(), [&os](auto& trget_and_msgs) {
    os << trget_and_msgs.second;
  });
  return os;
}
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


// Porque cuando agrego & me tira un error?
string mk_instr(Rank target,const string& sender,const string& body) {
    return "SEND " + sender + " " + to_string(target) + " " + body;
}

/**
 * @brief Takes a JingleNet and a message and sends the message to every target
 * 
 * @param sys the messaging system
 * @param sender the sender of the message
 * @param body the content of the message
 */
void send_msg_to_everyone(JingleNet& sys,const string& sender,const string& body) {
    for_each(receivers.begin(), receivers.end(), 
    [&sys, &sender, &body](Rank r) {sys.apply_instruction(mk_instr(r, sender, body));});
}

typedef vector<Message> msgs;
AllMessages mk_msg_coll(msgs snowman, msgs elf2, msgs elf1, msgs reindeer, msgs santa) {
    return {{Rank::SANTA, santa}, {Rank::ELF1, elf1}, {Rank::ELF2, elf2}, {Rank::REINDEER, reindeer}, {Rank::SNOWMAN, snowman}};
    }


TEST_CASE("JingleNet") {
//     SUBCASE("send") {
//         SUBCASE("An empty JingleNet has not messages") {
//             GIVEN("An empty JingleNet") {
//               JingleNet system;
//               THEN("All queues are empty") {
//                 AllMessages msgs = all_messages(system);
//                 REQUIRE(has_no_messages(msgs));
//               }
//             }
//         }
//         SUBCASE("Sending a message adds it to the queue ") {
//             GIVEN("An empty JingleNet") {
//               JingleNet system;
//               WHEN("Sending a message to santa") {
//                 string instruction = "SEND a santa hi";
//                 system.apply_instruction(instruction);
//                 THEN("The santa queue has the message") {
//                   vector<Message> expected{Message("a", "hi")};
//                   Rank target = Rank::SANTA;
//                   vector<Message> actual = messages_for(system, target);
//                   REQUIRE(expected == actual);
//                 }
//               }
//             }
//         }
//         SUBCASE("Sending two messages to santa"){
//             GIVEN("An empty JingleNet"){JingleNet sys;
//         WHEN("Sending two messages to santa") {
//             string instr_1 = "SEND a santa 1";
//             string instr_2 = "SEND a santa 2";
//             sys.apply_instruction(instr_1);
//             sys.apply_instruction(instr_2);
//             THEN("The santa queue has the two messages") {
//               vector<Message> expected{Message("a", "1"), Message("a", "2")};
//               Rank target = Rank::SANTA;
//               vector<Message> actual = messages_for(sys, target);
//               REQUIRE(expected == actual);
//             }
//         }
//     }
//   };
//   }
//   SUBCASE("announce") {
//       SUBCASE("Announcing messages when there are none has no effect") {
//           GIVEN("An empty JingleNet") {
//               JingleNet sys;
//               WHEN("A message is announced") {
//                   string announce_msg = "ANNOUNCE 1";
//                   sys.apply_instruction(announce_msg);
//                   THEN("The queues remained unchanged") {
//                      AllMessages msgs = all_messages(sys);
//                      REQUIRE(has_no_messages(msgs));
//                   }
//               }
//           }
//       }
//       SUBCASE("Announcing a single message will print it to announcements.txt") {
//         GIVEN("A empty JingleNet") {
//               JingleNet sys;
//               WHEN("A message is added and announced") {
//                 string instr_send = "SEND a santa 1";
//                 string instr_announce = "ANNOUNCE 1";
//                 sys.apply_instruction(instr_send);
//                 sys.apply_instruction(instr_announce);
//                 THEN("There will be a messsage in announcements.txt and the santa queue will be empty") {
//                      AllMessages msgs = all_messages(sys);
//                      REQUIRE(has_no_messages(msgs));

//                 }
//               }
//         }
//       }
//       SUBCASE("Announcing a message from every queue will print five messages to announcements.txt") {
//         GIVEN("An empty JingleNet") {
//             JingleNet sys;
//             WHEN("A message is sent to every target and five messages are announced") {
//                 string sender("b");
//                 string msg("1");
//                 send_msg_to_everyone(sys, sender, msg);
//                 sys.apply_instruction("ANNOUNCE 5");
//                 THEN("There will be no messages in any queue and five messages in announcements.txt") {
//                      AllMessages msgs = all_messages(sys);
//                      REQUIRE(has_no_messages(msgs));
//                 }
//             }
//         }
//       }
//       SUBCASE("Announcing a sole message for the reindeers will print only that message to announcements.txt") {
//         GIVEN("An empty JingleNet") {
//             JingleNet sys;
//             WHEN("A message is sent to the reindeers and three messages are announced") {
//                 sys.apply_instruction("SEND a reindeer hello");
//                 sys.apply_instruction("ANNOUNCE 3");
//                 THEN("There will be no messages in any queue and there will be a single message in announcements.txt") {
//                      AllMessages msgs = all_messages(sys);
//                      REQUIRE(has_no_messages(msgs));
//                 }
//             }
//         }
//       }
//   }
//   SUBCASE("remove_all") {
//     SUBCASE("Removing messages from an empty JingleNet does nothing") {
//         GIVEN("An empty JingleNet") {
//             JingleNet sys;
//             WHEN("Removing messages from a specific sender") {
//                 sys.apply_instruction("REMOVE_ALL jay");
//                 THEN("The JingleNet should remain unchanged") {
//                      AllMessages msgs = all_messages(sys);
//                      REQUIRE(has_no_messages(msgs));
//                 }

//             }
//         }
//     }
//   SUBCASE("Removing messages from a specific sender returns the JingleNet without messages sent by that sender") {
//     GIVEN("A JingleNet containing two messages for santa") {
//         JingleNet sys;
//         sys.apply_instruction("SEND A santa 1");
//         sys.apply_instruction("SEND B santa 2");
//         WHEN("All the messages with A as a sender are removed") {
//             sys.apply_instruction("REMOVE_ALL A");
//             THEN("Only the messages from B should be left in the queue") {
//                 AllMessages actual = all_messages(sys);
//                 vector<Message> empty;
//                 vector<Message> santa_msgs{Message("B", "2")};
//                 AllMessages expected = mk_msg_coll(empty, empty, empty, empty, santa_msgs);
//                 REQUIRE(expected == actual);
//             }
//         }
//     }
//   }
//   SUBCASE("Sending the same message to everyone and then removing that message returns an empty JingleNet") {
//     GIVEN("A JingleNet which has the same message for every receipient") {
//         JingleNet sys;
//         send_msg_to_everyone(sys, "a", "1");
//         WHEN("Removing the messages with a as a sender") {
//             sys.remove_all("a");
//             THEN("The queue for every recipient should be empty") {
//                 AllMessages actual = all_messages(sys);
//                 REQUIRE(has_no_messages(actual));
//             }
//         }
//     }
//   }
//   }
  SUBCASE("promote_announcements") {
    // SUBCASE("Promoting messages on an empty JingleNet does nothing") {
    //     GIVEN("An empty JingleNet") {
    //         JingleNet sys;
    //         WHEN("Promoting the messsages of a specific user") {
    //             sys.promote_messages("a");
    //             THEN("All queues will be remain unchanged and be empty") {
    //                 AllMessages actual = all_messages(sys);
    //                 REQUIRE(has_no_messages(actual));
    //             }
    //         }
    //     }
    // }
    SUBCASE("Promoting an existing message will move it to the queue above") {
        GIVEN("A JingleNet with a message for the reindeers") {
            JingleNet sys;
            sys.apply_instruction("SEND a reindeer 1");
            WHEN("Promoting the messages sent by a") {
                // sys.apply_instruction("PROMOTE_ANNOUNCEMENTS a");
                sys.promote_messages("a");
                THEN("The message is moved to the santa queue") {
                    AllMessages actual = all_messages(sys);
                    vector<Message> empty;
                    vector<Message> santa_msgs{Message("a", "1")};
                    AllMessages expected = mk_msg_coll(empty, empty, empty, empty, santa_msgs);
                    REQUIRE(expected == actual);
                }
            }
        }
    }
  }
  }