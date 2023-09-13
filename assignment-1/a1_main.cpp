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

    Test("Add used words"); 
    lst.add_word("hello");
    assert(lst.length() == 2);

    // Add more test cases here. Use assert or if-statements to automatically
    // check the correctness of your code.
    //

} // test_Wordlist

// void test_Wordlist_copy_constructor() {

//     Wordlist src;
//     src.add_word("1");
//     src.add_word("2");
//     Wordlist copy(src);
//     assert(copy.length() == 2);
// }

int main()
{
    test_Wordlist_default_constructor();
    // test_Wordlist_copy_constructor();
    // read_from_terminal();
}
