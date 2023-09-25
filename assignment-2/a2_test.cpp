#include "Stringlist.h"
#include <vector>
#include <cassert>
using namespace std;

/**
 * @brief  Takes a list, and returns a vector of all the words in the list.
 * 
 * @param lst The list being passed
 * @return vector<string> All the words in the list
 */
vector<string> as_vector(Stringlist& lst) {
  vector<string> words;
  for (int i = 0; i < lst.size(); i++) {
    words.push_back(lst.get(i));
  }
  return words;
}

/**
 * @brief Takes a list and the number of times to undo an action N, 
 * and undoes the list N times.
 * 
 * @param undoes The amount of times to undo the list
 * @param lst The list being passed
 */
void undo_many(int undoes, Stringlist& lst) {
    for (int i = 0; i < undoes; i++) {
        assert(lst.undo());
    }
}


bool is_empty_list(Stringlist& lst) {
    return as_vector(lst).empty();
}

auto mk_vector(initializer_list<string> words) {
    return vector<string>(words);
}

struct Test
{
    string name;
    Test(const string &name)
        : name(name)
    {
        cout << "Calling " << name << " ...\n";
    }

    ~Test()
    {
        cout << "... " << name << " done: all tests passed\n";
    }
}; // struct Test

void undo_insert_before() {
    Stringlist lst;
    Test("Undoing the insertion of an item removes the item from the list");
    lst.insert_before(0, "1");
    assert(as_vector(lst) == mk_vector({"1"}));
    assert(lst.size() == 1);

    assert(lst.undo());
    assert(lst.empty());

    Test(
        "Adding two items and undoing the last insertion should leave a list "
        "with the first item");
    lst.insert_before(0, "first");
    lst.insert_before(0, "second");
    assert(lst.size() == 2);
    assert(as_vector(lst) == mk_vector({"second", "first"}));
    
    assert(lst.undo());
    assert(as_vector(lst) == mk_vector({"first"}));
}

    void undo_remove_at() {
    Test(
        "Undoing the removal of a word should return it back to its original "
        "location");
    Stringlist lst;
    lst.insert_before(0, "1");
    lst.remove_at(0);
    assert(lst.empty());

    lst.undo();
    assert(as_vector(lst) == mk_vector({"1"}));

    Test("Undoing the removal of an entire list returns the list to its original state");
    lst.insert_before(1, "2");
    lst.insert_before(2, "3");
    assert(as_vector(lst) == mk_vector({"1", "2", "3"}));

    lst.remove_at(0);
    lst.remove_at(0);
    lst.remove_at(0);
    assert(lst.empty());

    undo_many(3, lst);
    assert(as_vector(lst) == mk_vector({"1", "2", "3"}));

    Test(
        "Undoing the removal of all the words starting from the end to the "
        "beginning returns the list in its original state");
    lst.remove_at(2);
    lst.remove_at(1);
    lst.remove_at(0);

    undo_many(3, lst);

    assert(as_vector(lst) == mk_vector({"1", "2", "3"}));
    }

    void undo_set() {
        Test(
            "Setting a word in the list and undoing that returns the word to "
            "its previous value");
        Stringlist lst;
        lst.insert_before(0, "1");
        assert(as_vector(lst) == mk_vector({"1"}));

        lst.set(0, "2");
        assert(as_vector(lst) == mk_vector({"2"}));

        assert(lst.undo());
        assert(as_vector(lst) == mk_vector({"1"}));

        Test(
            "Changing a collection of words and undoing all those changes "
            "returns the list to its original state");
        lst.insert_back("2");
        lst.insert_back("3");

        assert(as_vector(lst) == mk_vector({"1", "2", "3"}));

        lst.set(0, "-1");
        lst.set(1, "-2");
        lst.set(2, "-3");
        assert(as_vector(lst) == mk_vector({"-1", "-2", "-3"}));

       undo_many(3, lst);

       assert(as_vector(lst) == mk_vector({"1", "2", "3"}));
    }

    void undo_no_changes() {
       Test(
           "Nothing happens when undoing an action on a list where nothing has "
           "occured");

       Stringlist lst;
       assert(!lst.undo());
       assert(lst.empty());


       Test(
           "Undoing the list when there are no actions left to undo does "
           "nothing");
       lst.insert_before(0, "1");

       assert(lst.undo());
       assert(lst.empty());

       assert(!lst.undo());
       assert(lst.empty());
    }

    void undo_insert_back() {
            Stringlist lst;
            Test(
                "Undoing the insertion of an element at the end of the list "
                "returns "
                "the list before the element was added");

            lst.insert_back("1");
            assert(lst.size() == 1);
            assert(as_vector(lst) == mk_vector({"1"}));

            assert(lst.undo());
            assert(lst.empty());

            Test(
                "Undoing all insertions to the end of the list returns an "
                "empty list");
            lst.insert_back("1");
            lst.insert_back("2");

            assert(lst.size() == 2);
            assert(as_vector(lst) == mk_vector({"1", "2"}));

            undo_many(2, lst);
            assert(lst.empty());
    }

    void undo_insert_front() {
        Stringlist lst;
        Test(
            "Undoing the insertion of a word to the front of the list returns "
            "the rest of the list");
        lst.insert_front("1");
        assert(as_vector(lst) == mk_vector({"1"}));

        assert(lst.undo());
        assert(lst.empty());

        Test(
            "Undoing multiple insertions to the front of the list returns the "
            "list before the insertions");

        lst.insert_front("1");
        lst.insert_front("2");
        assert(as_vector(lst) == mk_vector({"2", "1"}));

        undo_many(2, lst);

        assert(lst.empty());
    }

    void undo_remove_first() {
        Stringlist lst;
        Test(
            "Removing the first occurence of a word and undoing that returns "
            "the list with the first appearance of the word in its original position");

        lst.insert_back("1");
        lst.insert_back("2");
        lst.insert_back("1");
        assert(as_vector(lst) == mk_vector({"1", "2", "1"}));

        assert(lst.remove_first("1"));
        assert(as_vector(lst) == mk_vector({"2", "1"}));

        assert(lst.undo());
        assert(as_vector(lst) == mk_vector({"1", "2", "1"}));

        Test(
            "Undoing the removal of a word not in the list will undo the "
            "previous action before the removal");
        assert(!lst.remove_first("3"));
        assert(as_vector(lst) == mk_vector({"1", "2", "1"}));
        assert(lst.undo());
        assert(as_vector(lst) == mk_vector({"1", "2"}));
    }

    void undo_remove_all() {
        Stringlist lst;

        Test(
            "Undoing the removal of all the words in a list with one word will "
            "return the "
            "original list");
            lst.insert_back("1");
            lst.remove_all();
            assert(lst.undo());
            assert(as_vector(lst) == mk_vector({"1"}));

            Test(
                "Undoing the removal of all the words in a larger list will "
                "return the original list");
            
           lst.insert_back("2");
           lst.insert_back("3");
           assert(as_vector(lst) == mk_vector({"1", "2", "3"}));

           lst.remove_all();
           assert(lst.undo());
           assert(as_vector(lst) == mk_vector({"1", "2", "3"}));

           Test(
               "Undoing the removal of all the words in an empty list will "
               "return an empty list");
           Stringlist empty_list;
           empty_list.remove_all();
           assert(empty_list.empty());
           assert(empty_list.undo());
           assert(empty_list.empty());
    }

    void undo_assignment_operator() {
        Stringlist src;
        Stringlist cpy;
        Test(
            "Undoing the assignment of a non-empty list to an empty list will "
            "return an empty list");
        src.insert_back("1");
        src.insert_back("2");

        cpy = src;
        assert(as_vector(cpy) == mk_vector({"1", "2"}));
        assert(cpy.undo());
        assert(as_vector(cpy).empty());

        {
        Test(
            "Assigning the same list to itself does not generate an undoable "
            "action");
        Stringlist empty;
        empty = *&empty;
        assert(!empty.undo());
        }

        {
        Test(
            "Undoing the assignment of an empty list to an an empty list will "
            "return an empty list");
        Stringlist empty_list;
        Stringlist empty_list2;
        assert(empty_list.empty());

        empty_list = empty_list2;
        assert(empty_list.empty());
        assert(empty_list.undo());
        assert(empty_list.empty());
        }

        {
        Test(
            "Undoing the assignment of a empty list to a non empty list will "
            "return "
            "the original non-empty list");
        Stringlist empty_list;
        Stringlist src;
        src.insert_back("1");
        src.insert_back("2");
        assert(as_vector(src) == mk_vector({"1", "2"}));

        src = empty_list;
        assert(as_vector(src).empty());

        assert(src.undo());
        assert(as_vector(src) == mk_vector({"1", "2"}));
        }

    }


int main() {
    undo_insert_before();
    undo_remove_at();
    undo_set();
    undo_no_changes();
    undo_insert_back();
    undo_insert_front();
    undo_remove_first();
    undo_remove_all();
    undo_assignment_operator();
}   
