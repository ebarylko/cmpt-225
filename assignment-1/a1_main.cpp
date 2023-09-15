// a1_main.cpp

//
// You can change this file in any way you like.
//
// Thoroughly test your Wordlist class to make sure it is correct.
//

//#include "Wordlist_sol.h"
#include "iostream"
#include "test.h"
#include "Wordlist.h"
using namespace std;

/* void read_from_terminal()
{
    Wordlist lst;
    string w;
    while (cin >> w)
    {
        lst.add_word(w);
    }

    // print the words in sorted order
    vector<string *> index = lst.get_sorted_index();
    for (int i = 0; i < index.size(); i++)
    {
        cout << (i + 1) << ". " << *index[i] << endl;
    }
} */

void test_Wordlist_default_constructor()
{
    Test("Default constructor");

    Wordlist lst;
    assert(!lst.is_frozen());
    assert(lst.length() == 0);
    assert(!lst.contains("hello"));

    Test("Add unique words");
    lst.add_word("hello");
    assert(lst.contains("hello"));
    assert(lst.length() == 1);
    assert(lst.first_word() == lst.last_word() && lst.last_word() == "hello");

    lst.add_word("hi");
    assert(lst.length() == 2);
    assert(lst.first_word() == "hello" && lst.last_word() == "hi");
    assert(lst.contains("hi"));

    Test("Adding used words only modifies the occurences of the word"); 
    lst.add_word("hello");
    assert(lst.length() == 2);
    assert(lst.word_occurences("hello") == 2);
    lst.add_word("hello");
    assert(lst.length() == 2);
    assert(lst.word_occurences("hello") == 3);
    lst.add_word("hello");
    assert(lst.length() == 2);
    assert(lst.word_occurences("hello") == 4);


    // Test("Deleting all the words in the list leaves an empty list");
    // lst.remove_word("hi");
    // assert(lst.is_empty());
    // Add more test cases here. Use assert or if-statements to automatically
    // check the correctness of your code.
    //

} // test_Wordlist

void remove_word() {
    Wordlist lst;
    lst.add_word("hello");
    lst.add_word("hello");
    lst.add_word("hi");

    Test("Deleting unsused words does nothing"); 
    lst.remove_word("General");
    assert(lst.length() == 2);
    assert(lst.word_occurences("hello") == 2);
    assert(lst.word_occurences("hi") == 1);

    Test("Deleting the first word in a list of two items changes the size and the first/last word");
    lst.remove_word("hello");
    assert(lst.length() == 1);
    assert(lst.first_word() == lst.last_word() && lst.first_word() == "hi");

    Test("Deleting the last item in a list returns an empty list");
    lst.remove_word("hi");
    assert(lst.is_empty());

    Test("Deleting a word inbetween the middle will link the words before and after the deleted word");
    lst.add_word("1");
    lst.add_word("2");
    lst.add_word("3");
    lst.remove_word("2");

    assert(lst.length() == 2);
    assert(lst.next_word(lst.first()) == "3");
    assert(lst.prev_word(lst.last()) == "1");

    Test("Deleting the last word in the list changes the last word");
    lst.remove_word("3");
    assert(lst.length() == 1);
    assert(lst.first_word() == "1");
    
}

void get_word_test() {
    Wordlist lst;
    lst.add_word("1");
    lst.add_word("2");
    lst.add_word("3");

   Test("Searching for the first, middle, and last word");
   assert(lst.first_word() == "1");
   assert(lst.get_word(1) == "2");
   assert(lst.last_word() == "3");


}

// void test_Wordlist_copy_constructor() {

//     Wordlist src;
//     src.add_word("1");
//     src.add_word("2");
//     Wordlist copy(src);
//     assert(copy.length() == 2);
// }

void copy_constructor() {
    Wordlist src;
    src.add_word("1");
    src.add_word("2");
    assert(src.length() == 2);
    assert(src.first_word() == "1");
    assert(src.last_word() == "2");
    assert(src.word_occurences("1") == 1);
    assert(src.word_occurences("2") == 1);

    Test("The copy constructor copies a list of unique words with no repitition  without modifying it");
    Wordlist copy(src);
    
    assert(src.length() == 2);
    assert(src.first_word() == "1");
    assert(src.last_word() == "2");
    assert(src.word_occurences("1") == 1);
    assert(src.word_occurences("2") == 1);

    // assert(copy.length() == 2);
    // assert(copy.first_word() == "1");
    // assert(copy.last_word() == "2");
    // assert(copy.word_occurences("1") == 1);
    // assert(copy.word_occurences("2") == 1);
}
int main()
{
    test_Wordlist_default_constructor();
    remove_word();
    get_word_test();
    copy_constructor();
    // test_Wordlist_copy_constructor();
    // read_from_terminal();
}
