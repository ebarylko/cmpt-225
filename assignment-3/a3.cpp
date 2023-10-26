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


/**
 * @brief 
 This class holds a 
 collection of items and 
 implements all the basic functions of a queue
 */
template <typename T>
class Queue : public Queue_base<T> {

  // This struct represents an item in the queue
  // which is able to go the next and previous item
  struct Node {
    Node* prev;
    T curr;
    Node* next;

    Node(T curr) :  prev(nullptr), curr(curr), next(nullptr){};
    ~Node() {};
  };

  /**
   * @brief The first and last item along with 
   * the amount of elements
   */
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

/**
 * @brief Returns the amount of elements in the queue
 * 
 * @return int the number of elements
 */
  int size() const {
     return this->elems; 
     }


/**
 * @brief Returns true if the queue is empty
 * 
 * @return true if queue has no elements
 * @return false if condition above false
 */
  bool is_empty() const {
    return !this->size();
  }

  /**
   * @brief Returns true if there are elements in the queue
   * 
   * @return true if the queue has elements
   * @return false if the above condition is false
   */
  bool has_items() const {
     return this->size(); 
     }

/**
 * @brief Takes an item and adds it to the end of the queue
 * 
 * @param item the item to add on to the end of the queue
 */
  void enqueue(const T& item) {
    Node* new_item = new Node(item);
    // setting the first and last element to
    // the new element if queue is empty
    if (this->is_empty()) {
      this->first = new_item;
      this->last = new_item;
    } else {
      // Adding the element after the last element
      this->last->next = new_item;
      new_item->prev = this->last;
      this->last = new_item;
    }
    this->elems++;
  }

/**
 * @brief Removes the element at the front of the queue
 * if the queue is not empty. Throws an error otherwise
 * 
 */
  void dequeue() {
    if (this->is_empty()) {
      throw runtime_error("dequeue: queue is empty");
    }
    Node* remove = this->first;
    this->first = remove->next;
    this->elems--;
    // Changing the first and last element of the
    // queue depending on how many items there are
    if (this->has_items()) {
        this->first->prev = nullptr;
    } else {
        this->last = nullptr;
    }

    delete remove;
  }


/**
 * @brief Returns the first element in the queue if nonempty. 
 * Throws an error otherwise
 * 
 * @return const T& the first element in the queue
 */
  const T& front() const {
    // Checking if the queue is empty before
    // returning the first element
    if (this->is_empty()) {
      throw runtime_error("front: queue is empty");
    }
    return this->first->curr;
  }

};

/**
 * @brief 
 JingleNet holds and manages the messages held by 
 santa, reindeer, elf2, elf1, and snowman
 It takes one of the four following instructions:
 SEND sender-name receiver-name message 
 REMOVE_ALL sender-name
 ANNOUNCE [number of messages]
 PROMOTE_ANNOUNCEMENTS sender-name
 JingleNet takes one of these instructions and then applies it.
 * 
 */
class JingleNet {
  /**
   * @brief
   This struct represents a message
   and contains a sender, receiver, and
   body
   */
  struct Message {
    typedef string Body, Sender, Receiver;
    Sender sender;
    Body content;
    Message() : sender(""), content(""){};
    Message(const Sender& from, const Body& content)
        : sender(from), content(content){};
  };

    /**
     * @brief 
     This struct is used for 
     iterating over an instruction and 
     extracting the command, username, and
     message content (if using send)
     */
    struct WordInfo {
    string found;
    string rest;
    };

/**
 * @brief Takes either a full or incomplete instruction and
 * returns the next word before a space and the rest of the
 * instruction
 * 
 * @param search_in the instruction to look over
 * @return WordInfo a portion of the original instruction 
 * and the rest of the instruction
 */
    WordInfo read_word(string search_in) {
    int space_pos = search_in.find_first_of(" ");
    WordInfo res;

    // Checking if a word has been found
    if (space_pos != -1) {
      res.found = search_in.substr(0, space_pos);
      res.rest = search_in.substr(space_pos + 1);
    }

    return res;
    }


/**
 * @brief Takes a message and a target and sends the message to the target
 * 
 * @param msg the message to send
 * @param to the recipient of the message
 */
     void send_message(const Message& msg, Rank to) {
        get_messages(to).enqueue(msg);
    }

  /**
   * @brief 
   The messages for santa, reindeer, elf2, elf1, snowman
   */
  Queue<Message> messages[5];

typedef Queue<Message>& MessageQueueRef;

  /**
   * @brief Takes a number of messages to remove N and a receiver (santa|reindeer|elf2|elf1|snowman)
   *  and removes N messages from the receiver's queue (less if the queue has less than N messages).
   *  Returns the amount of messages left to remove in the other queues
   *
   * @param msgs_to_announce the number of messages to announce
   * @param target the receiver to remove messages from
   * @return int the amount of messages to remove in the other queues
   */
  int announce_n(int msgs_to_announce, Rank target) {
        Message announcing;
        MessageQueueRef to_remove = this->get_messages(target);

        // Announcing messages while there messages left in 
        // the queue and more messages need to be announced
        while (to_remove.has_items() && msgs_to_announce != 0) {
            announcing = to_remove.front();
            Announcement a(announcing.sender, target, announcing.content);
            jnet.announce(a);
            to_remove.dequeue();
            msgs_to_announce--;
        }

        return msgs_to_announce;
  }

/**
 * @brief 
 A list of all the receivers
 */
Rank receiver[5] = {Rank::SNOWMAN, Rank::ELF1, Rank::ELF2, Rank::REINDEER, Rank::SANTA};
Rank* snowman = receiver;
Rank* santa = receiver + 4;

/**
 * @brief takes a number of messages to announce N and announces N messages in total from the queue
 * in this order: santa -> reindeer -> elf2 -> elf1 -> snowman. For each queue, it announces
 * N - (the amount of messages from the previous queues), meaning if I have 
 * five messages in the santa queue and two messages in the reindeer queue, announcing five
 * messages will remove only the santa messages.
 * @param num the number of messaages to announce
 */
void announce_msgs(int num) {
    // Announcing the messages starting from the santa queue
    // and announcing the messages in the earlier queue if 
    // there are messages left
    for (Rank* curr = santa; curr != snowman - 1; curr--) {
            num = this->announce_n(num, *curr);
    }
}

/**
 * @brief Takes a target and returns the messages for that target
 * 
 * @param to the target to search
 * @return MessageQueueRef the messages the target has
 */
  MessageQueueRef get_messages(Rank to) {
        return messages[to_int(to) - 1];
  }

/**
 * @brief Takes a message and a sender and returns true if 
 * the message came from the sender. False otherwise
 * 
 * @param msg the message to check
 * @param sender the potential sender of the message
 * @return true if the message came from the sender
 * @return false if the above is not true
 */
bool was_sent_from(const Message& msg, const string& sender) {
    return msg.sender == sender;
}

/**
 * @brief Takes two message collections, src and dest, and moves all the 
 * messages from src to dest
 * 
 * @param src the collection to move from
 * @param dest the collection to move messages to
 * @return MessageQueueRef 
 */
MessageQueueRef mv_msgs(MessageQueueRef src, MessageQueueRef dest) {
    // Moving the messages from src to dest
    while (src.has_items()) {
        dest.enqueue(src.front());
        src.dequeue();
    }
    return dest;
}

/**
 * @brief Takes a message collection and a sender, and removes the 
 * messages from the collection that were sent from that sender
 * 
 * @param src the message collection to look through
 * @param sender the sender whose messages will be removed
 * @return MessageQueueRef the collection without the messages
 * sent by sender
 */
MessageQueueRef remove_msgs(MessageQueueRef src, const string& sender) {
    Queue<Message> cpy;
    Message msg_to_check;
    // Copying the messages that were not sent from the sender 
    while (src.has_items()) {
        msg_to_check = src.front();
        if (!was_sent_from(msg_to_check, sender)) {
            cpy.enqueue(msg_to_check);
        }
        src.dequeue();
    }
    // Returning the original list without the 
    // messages from the sender
   return mv_msgs(cpy, src);
}

/**
 * @brief Takes a sender name S and removes all messages with S as the sender
 * 
 * @param sender the sender whose messages will be removed
 */
void remove_all(const string& sender) {
    // Removing the messages originating from the sender
    // in every queue
    for (Rank* curr = snowman; curr != santa + 1; curr++) {
            this->remove_msgs(get_messages(*curr), sender);
    }
}

typedef Queue<Message> MsgQueue;

/**
 * @brief Takes a collection of messages and a sender and moves
 * all the messages with that specific sender to the next highest queue
 * 
 * @param coll the collection to look over
 * @param sender the sender whose messages we will be moving
 */
void mv_msgs_from(const string& sender, Rank src, Rank target) {
    // Getting the messages from the 
    // origin and destination targets
    MsgQueue& from = this->get_messages(src);
    MsgQueue& to = this->get_messages(target);
    MsgQueue tmp;

    // Moving the messages from the origin to the destination
    while (from.has_items()) {
        Message msg = from.front();
        if (was_sent_from(msg, sender)) {
            to.enqueue(msg);
        } else {
            tmp.enqueue(msg);
        }
        from.dequeue();
    }
    mv_msgs(tmp, from);
}

/**
 * @brief Takes a sender and returns 
 * the value associated with that sender
 * 
 * @param num the sender to convert
 * @return int the value of that sender
 */
int to_int(Rank num) {
    return static_cast<int>(num);
}

/**
 * @brief Takes a value and returns the receiver associated with
 * that value
 * 
 * @param num the value of the receiver
 * @return Rank the receiver asoociated with that value
 */
Rank from_int(int num) {
    return receiver[num - 1];
}

/**
 * @brief Takes a receiver and returns the previous receiver
 * following this order: snowman <- elf1 <- elf2 <- reindeer <- santa
 * 
 * @param src the original receiver
 * @return Rank the previous receiver
 */
Rank previous(Rank src) {
    return from_int(to_int(src) - 1);
}


/**
 * @brief Takes a receiver and returns the next receiver
 * following this order: snowman -> elf1 -> elf2 -> reindeer -> santa
 * 
 * @param src the original receiver
 * @return Rank the next receiver
 */
Rank next(Rank src) {
    return from_int(to_int(src) + 1);
}

/**
 * @brief Takes a sender and moves all the messsages from that sender one 
 * queue higher following this order: snowman -> elf1 -> elf2 -> reindeer -> santa
 * @param sender the sender of the messages to move
 */
void promote_messages(const string& sender) {
    // Moving messages from original queue to the target queue following the order above
    for(Rank* curr = santa - 1; curr != snowman - 1; curr--) {
        Rank src = *curr;
        mv_msgs_from(sender, src, next(src));
    }
}

 public:
  ~JingleNet(){};
  /**
   * @brief Takes an instruction and applies the instruction onto the JingleNet
   * system
   *
   * @param instruction One of four possible commands to be executed by
   * JingleNet
   */
  void apply_instruction(const string& instruction) {
    string message;
    // Getting the details of the instruction
    // before checking what kind of instruction it is
    WordInfo instr = read_word(instruction);
    string send_instr = "SEND";
    string announce_instr = "ANNOUNCE";
    string remove_all_instr = "REMOVE_ALL";
    string promote_instr = "PROMOTE_ANNOUNCEMENTS";
    // Sending a message 
    if (send_instr == instr.found) {
      WordInfo sender = read_word(instr.rest);
      WordInfo receiver = read_word(sender.rest);
      Message msg(sender.found, receiver.rest);
      Rank to = to_rank(receiver.found);
      this->send_message(msg, to);
    // Announcing messages 
    } else if (announce_instr == instr.found) {
        int announce_num = stoi(instr.rest);
        this->announce_msgs(announce_num);
        // Removing all messages containing the sender
    } else if (remove_all_instr == instr.found) {
        this->remove_all(instr.rest);
    // Promoting all the messages from the sender
    } else if (promote_instr == instr.found) {
        this->promote_messages(instr.rest);
    }
  }
};

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1; 
    }
    string filename = argv[1];
    ifstream input_file(filename);
    string instruction;

    JingleNet sys;
    while(getline(input_file, instruction)) {
        sys.apply_instruction(instruction);
    }
  return 0;
}

