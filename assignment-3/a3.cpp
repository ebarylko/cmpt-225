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
#include <vector>
#include <map>
#include <sstream>

using namespace std;


// Dos tipos de insrucciones. 
// una que modifica la lista entera
// y la otra que solo modifica un queue

template <typename T> class Queue : public Queue_base<T> {

    struct Node {
       Node* prev;
       T curr;
       Node* next;
    };

    Node* first;
    Node* last;
    int elems;

    public :

    Queue<T>(): first(nullptr), last(nullptr), elems(0) {};
    ~Queue<T>() {
        while (!this->is_empty()) {
            this->dequeue();
        }
    };

    int size() const {
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

    stringstream& print_items() {
        stringstream items;
        Node* curr_itm = this->first;
        while (curr_itm) {
            items << curr_itm->curr;
            curr_itm = curr_itm->next;
        }
        return items

    };

    const T& front() const {
        if (this->is_empty()) {
            throw runtime_error("front: queue is empty");
        }
        return this->first->curr;
    }
};      

// quiero esconder el struct, before el operador de << tiene que esta afuera
struct Message {
    typedef string Body, Sender, Receiver;
    Sender sender;
    Receiver recipient;
    Body content;
    friend ostream& operator<<(ostream& os, Message& msg);
};

ostream& operator <<(ostream& os, Message& msg) {
    os << "[" << msg.sender << ' ';
    os << msg.recipient << ' ';
    os << msg.content << "] ";
    return os;
}



// JingleNet has 
// a queue for each of the following:
// santa, reindeer, elf2, elf1, snowman
class JingleNet {
    enum receiver { Santa, Reindeer, Elf2, Elf1, Snowman };


    // Base class for an instruction which contains a method for executing a
    // specific instruction
    class Instruction {
        public:
        virtual void execute(JingleNet system) = 0;
        virtual ~Instruction();
    };

    // Enqueues the message from the sender to the queue where the receiver is
    class Send : public Instruction {
        Message msg;


        public:

        Send(Message new_msg): msg(new_msg){};

        void set_message(Message msg) {
            this->msg = msg;
        }

        virtual void execute(JingleNet system) {
            // Checks where the message must be sent
            if (msg.recipient == "santa") {
              system.messages[Santa].enqueue(msg);
            } else if (msg.recipient == "reindeer") {
              system.messages[Reindeer].enqueue(msg);
            }

            else if (msg.recipient == "elf2") {
              system.messages[Elf2].enqueue(msg);
            }

            else if (msg.recipient == "elf1") {
              system.messages[Elf1].enqueue(msg);
            }

            else if (msg.recipient == "snowman") {
              system.messages[Snowman].enqueue(msg);
            }
        }
    };

/**
 * @brief Takes the details of a message and returns a message
 * 
 * @param message_info a string with information about the sender, receiver, and message content
 * @return Message a message containing all the information in message_info
 */
Message fill_message(char* message_info) {
    Message new_msg;
    string body;

    // Setting the information of the message
    message_info = strtok(NULL, " ,.-");
    new_msg.sender = message_info;

    message_info = strtok(NULL, " ,.-");
    new_msg.recipient = message_info;

    message_info = strtok(NULL, " ,.-");
    while (message_info) {
            body += message_info;
            message_info = strtok(NULL, " ,.-");
    }

    new_msg.content = body;
    return new_msg;
}

    Queue<Message> santa_msgs, reindeer_msgs, elf1_msgs, elf2_msgs, snowman_msgs;
    Queue<Message> messages[5] = {santa_msgs, reindeer_msgs, elf1_msgs, elf2_msgs, snowman_msgs};

    public:
/**
 * @brief Takes an instruction and applies the instruction onto the JingleNet system
 * 
 * @param instruction One of four possible commands to be executed by JingleNet
 */
void apply_instruction(string instruction) {
        string message;
        char* sentence = const_cast<char*>(instruction.c_str());
        char* word = strtok(sentence, " ");

        // Send message if the instruction was send
        if (word == "SEND") {
            Message msg = fill_message(word);
            Send send = Send(msg);
            send.execute(*this);
        }
}

/**
 * @brief Takes a receiver and returns all the messages for that person
 * 
 * @param receiver one of the following targets: santa, reindeer, elf2, elf1, snowman
 * @return string all the messages the target has received
 */
string const messages_for(string receiver) {
    map<string, int> name_to_pos = {
      {"santa", Santa},
      {"reindeer", Reindeer},
      {"elf2", Elf2},
      {"elf1", Elf1},
      {"snowman", Snowman}
    };
    int pos = name_to_pos[receiver];
    Queue<Message> look_in = this->messages[pos];
    stringstream& messages = look_in.print_items();
    return messages.str();
}
};

// parsiar una instruccion:
// ir linea por linea, y crear commandos
// applicar commandos sobre las lineas


bool parse_and_apply_instruction_test() {
    JingleNet system;
    string instruction = "SEND a santa hi";
    system.apply_instruction(instruction);
    string expected = "a santa hi";
    string actual = system.messages_for("santa");
}


int main()
{
    cout << "Welcome to Assignment 3!" << endl;
}
