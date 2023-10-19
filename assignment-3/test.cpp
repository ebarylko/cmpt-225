#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
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
#include "test.h"
#include "doctest.h"

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

template <typename T> Queue<T>::Node::Node(T curr): curr(curr) {};


template <typename T> Queue<T>::Queue() : first(nullptr), last(nullptr), elems(0){};
template <typename T> Queue<T>::~Queue() {
    while (!this->is_empty()) {
    this->dequeue();
    }
};

template <typename T> int Queue<T>::size() const { return this->elems; }

template <typename T> bool Queue<T>::is_empty() const {
    return !this->size();
}

template <typename T> void Queue<T>::enqueue(const T& item) {
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

template <typename T> void Queue<T>::dequeue() {
    if (this->is_empty()) {
    throw runtime_error("dequeue: queue is empty");
    }
    Node* remove = this->first;
    this->first = this->first->next;
    this->elems--;
    this->last = this->is_empty() ? nullptr : this->last;
    delete remove;
}

template <typename T> vector<T> Queue<T>::print_items() {
    stringstream items;
    vector<T> itms;
    Node* curr_itm = this->first;
    while (curr_itm) {
    itms.push_back(curr_itm->curr);
    curr_itm = curr_itm->next;
    }
    return itms;
};

template <typename T> const T& Queue<T>::front() const {
    if (this->is_empty()) {
    throw runtime_error("front: queue is empty");
    }
    return this->first->curr;
}

void JingleNet::send_message(const Message& msg, Rank to) {
    get_messages(to).enqueue(msg);
}

Queue<Message>& JingleNet::get_messages(const Rank& to) {
    return messages[(int)to - 1];
}

/**
 * @brief Takes a receiver and returns all the messages for that person
 *
 * @param receiver one of the following targets: santa, reindeer, elf2, elf1,
 * snowman
 * @return string all the messages the target has received
 */
vector<Message> const JingleNet::messages_for(const Rank& receiver) {
    return this->get_messages(receiver).print_items();
  }

  /**
   * @brief Takes an instruction and applies the instruction onto the JingleNet
   * system
   *
   * @param instruction One of four possible commands to be executed by
   * JingleNet
   */
void JingleNet::apply_instruction(const string& instruction) {
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

Message::Message(const Sender& from, const Body& content)
    : sender(from), content(content){};

ostream& operator<<(ostream& os, const Message& msg) {
  os << msg.sender << ' ';
  os << msg.content;
  return os;
}

bool operator==(const Message& one, const Message two) {
    return one.content == two.content && one.sender == two.sender;
}

void test_case(string scenario) { cout << scenario << endl; }

// vector<Message> mk_msgs(initializer_list<Message>) {

// }
void print_message(Message msg) {
    cout << msg << endl;
}

void print_messages(vector<Message> messages) {
    cout << "here are the messages" << endl;
    for_each(messages.begin(), messages.end(), print_message );
}

namespace doctest {
// template<> struct StringMaker<Message> {
//     static string convert(const Message& msg) {
//     stringstream os;
//     os << "[";
//     os << msg;
//     os << "]";
//     return os.str().c_str();
//     }
// };

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
}  // namespace doctest



TEST_CASE("Testing all the methods of JingleNet"){
    SUBCASE("Testing send"){
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
}
// TEST_CASE( "vectors can be sized and resized") {

//     std::vector<int> v( 5 );

//     REQUIRE( v.size() == 5 );
//     REQUIRE( v.capacity() >= 5 );

//     SUBCASE( "resizing bigger changes size and capacity" ) {
//         v.resize( 10 );

//         REQUIRE( v.size() == 10 );
//         REQUIRE( v.capacity() >= 10 );
//     }
//     SUBCASE( "resizing smaller changes size but not capacity" ) {
//         v.resize( 0 );

//         REQUIRE( v.size() == 0 );
//         REQUIRE( v.capacity() >= 5 );
//     }
//     SUBCASE( "reserving bigger changes capacity but not size" ) {
//         v.reserve( 10 );

//         REQUIRE( v.size() == 5 );
//         REQUIRE( v.capacity() >= 10 );
//     }
//     SUBCASE( "reserving smaller does not change size or capacity" ) {
//         v.reserve( 0 );

//         REQUIRE( v.size() == 5 );
//         REQUIRE( v.capacity() >= 5 );
//     }
// }

//   TEST_CASE("Adding two messages for santa creates a queue with two messages that has the oldest message first") {
//   JingleNet sys;
//   string instr_1 = "SEND a santa 1";
//   string instr_2 = "SEND a santa 2";

//   sys.apply_instruction(instr_1);
//   sys.apply_instruction(instr_2);
//   vector<Message> expected{Message("a", "1"), Message("a", "2")};
//   Rank target = Rank::SANTA;
//   vector<Message> actual = sys.messages_for(target);
//   REQUIRE(expected == actual);
//   };

// int main() {
//     return 0;
// }

