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

auto mk_vector(initializer_list<string> words) {
    return vector<string>(words);
}

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
    assert(lst.as_vector().size() == 0);

    Test("Add unique words");
    lst.add_word("hello");
    assert(lst.contains("hello"));
    assert(lst.length() == 1);
    assert(lst.as_vector() == mk_vector({"hello"}));

    lst.add_word("hi");
    assert(lst.contains("hi"));
    assert(lst.length() == 2);
    assert(lst.as_vector() == mk_vector({"hello", "hi"}));

    Test("Adding used words only modifies the occurences of the word"); 
    lst.add_word("hello");
    assert(lst.length() == 2);
    lst.add_word("hello");
    assert(lst.length() == 2);
    lst.add_word("hello");
    assert(lst.length() == 2);


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

    Test("Deleting the first word in a list of two items changes the size and the first/last word");
    lst.remove_word("hello");
    assert(lst.length() == 1);
    assert(lst.as_vector() == mk_vector({"hi"}));

    Test("Deleting the last item in a list returns an empty list");
    lst.remove_word("hi");
    assert(lst.is_empty());

    Test("Deleting a word inbetween the middle will link the words before and after the deleted word");
    lst.add_word("1");
    lst.add_word("2");
    lst.add_word("3");
    lst.remove_word("2");

    assert(lst.length() == 2);
    assert(lst.as_vector() == mk_vector({"1", "3"}));

    Test("Deleting the last word in the list changes the last word");
    lst.remove_word("3");
    assert(lst.length() == 1);
    assert(lst.as_vector() == mk_vector({"1"}));

    Test("Adding the same word with different capatilization adds two words to the list");
    lst.add_word("hi");
    lst.add_word("Hi");
    assert(lst.length() == 3);
    assert(lst.as_vector() == mk_vector({"1", "hi", "Hi"}));
    
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
    assert(src.as_vector() == mk_vector({"1", "2"}));

    Test("The copy constructor copies a list of unique words with no repitition  without modifying it");
    Wordlist copy(src);
    
    assert(src.length() == 2);
    assert(src.as_vector() == mk_vector({"1", "2"}));

    assert(copy.length() == 2);
    assert(copy.as_vector() == mk_vector({"1", "2"}));
    
}

void freze_list() {
    Wordlist src;
    src.add_word("2");
    src.add_word("1");
    auto actual = src.get_sorted_index();
    vector<string> actual_strings;
    Test("The amount of words ordered alphabetically match the amount in the original list");

    transform(actual.begin(), actual.end(), back_inserter(actual_strings),
              [](string* word) { return *word; });
    assert(actual_strings == vector<string>({"1", "2"}));
}

void file_constructor() {
    Test(
        "The words inside a list which use a file as input contain the unique "
        "words inside the file");
    Wordlist lst("small.txt");
    assert(lst.length() == 7);
    assert(lst.as_vector() == mk_vector({"This", "is", "a", "test", "or", "this", "test?"}));

    auto actual = lst.get_sorted_index();
    vector<string> actual_words;
    transform(actual.begin(), actual.end(), back_inserter(actual_words),
              [](string* word) { return *word; });
    assert(actual_words == mk_vector({"This", "a", "is", "or", "test", "test?", "this"}));

    Test(
        "Using the file constructor on a file with 116 words creates a list "
        "with 90 words");
    Wordlist large_file("sonnet30.txt");
    assert(large_file.length() == 90);
    auto actual_larger_text = large_file.get_sorted_index();
    assert(actual_larger_text.size() == 90);

    ofstream file("sonnet30_words.txt");
    vector<string> sonnet_actual_words;
    transform(actual_larger_text.begin(), actual_larger_text.end(), back_inserter(sonnet_actual_words),
              [](string* word) { return *word; });

    for (auto i = 0; i < actual_larger_text.size(); i++) {
        file << i + 1 << ". " << sonnet_actual_words[i] << "\n";
    }
    file.close();

    Wordlist largest_file("tiny_shakespeare.txt");
    assert(largest_file.length() == 25670);
    auto actual_shakespeare_text = largest_file.get_sorted_index();
    ofstream shakespeare("tiny_shakespeare_words.txt");
    vector<string> shakespeare_actual_words;
    transform(actual_shakespeare_text.begin(), actual_shakespeare_text.end(), back_inserter(shakespeare_actual_words),
              [](string* word) { return *word; });

    for (auto i = 0; i < actual_shakespeare_text.size(); i++) {
        shakespeare << i + 1 << ". " << shakespeare_actual_words[i] << "\n";
    }

}


int main()
{
    test_Wordlist_default_constructor();
    remove_word();
    get_word_test();
    copy_constructor();
    freze_list();
    file_constructor();
    // test_Wordlist_copy_constructor();
    // read_from_terminal();
}
