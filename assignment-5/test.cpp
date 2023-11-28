#define TESTING 1
#include "Wordlist.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <ostream>

typedef WordlistTest::Heights Heights;
typedef WordlistTest::SubtreeHeights node_heights;
template <typename T1, typename T2> 
ostream& operator<<(ostream& os, const pair<T1, T2>& heights) {
    os << "[" << heights.first << ", " << heights.second << "]";
    return os;
}


typedef WordlistTest::ListData ListData;

namespace doctest
{

template <typename T> struct StringMaker<vector<T>> {
     static String convert(const vector<T>& in) {
        stringstream os;
        os << "[";
        for (auto it = in.cbegin(); it != in.cend();) {
            os << *it;
            if (++it != in.cend()) { os << ", "; }
        }
        os << "]";
        return os.str().c_str();
}
};


template <>
struct StringMaker<vector<string>>
{
    static String convert(const vector<string>& in) {
        ostringstream oss;

        oss << "[";
        for (auto it = in.cbegin(); it != in.cend();) {
            oss << *it;
            if (++it != in.cend()) { oss << ", "; }
        }
        oss << "]";
        return oss.str().c_str();
    }
};


template <>
struct StringMaker<ListData>
{
    static String convert(const ListData& in) {
        ostringstream oss;

        oss << "[";
        oss << "(" << get<0>(in) << ", " << get<1>(in) << ", " << get<2>(in) << ", " << get<3>(in) << ", " << get<4>(in)  << ")";

        oss << "]";
        return oss.str().c_str();
    }
};

template <>
struct StringMaker<vector<pair<int, int>>>
{
    static String convert(const vector<pair<int, int>>& in) {
        ostringstream oss;

        oss << "[";
        for (auto it = in.cbegin(); it != in.cend();) {
            oss << "(" << it->first << ", " << it->second << ")";
            if (++it != in.cend()) { oss << ", "; }
        }
        oss << "]";
        return oss.str().c_str();
    }
};

template <>
struct StringMaker<vector<pair<string, int>>>
{
    static String convert(const vector<pair<string, int>>& in) {
        ostringstream oss;

        oss << "[";
        for (auto it = in.cbegin(); it != in.cend();) {
            oss << "(" << it->first << ", " << it->second << ")";
            if (++it != in.cend()) { oss << ", "; }
        }
        oss << "]";
        return oss.str().c_str();
    }
};
}

/**
 * @brief Takes a collection of expected heights and creates a collection of 
 * heights of the nodes visited using inorder traversal
 * 
 * @param all_heights the expected heights
 * @return heights the collection of heights corresponding to the nodes
 * in alphabetical order
 */
Heights mk_heights(initializer_list<pair<int,int>> all_heights) {
    Heights tree_heights{all_heights};
    return tree_heights;
}

typedef vector<string> Words;

TEST_CASE("WordlistTest") {
    SUBCASE("Empty WordlistTest has no words") {
        GIVEN("An empty WordlistTest") {
            WordlistTest lst;
            WHEN("We check the number of words") {
                THEN("There should be none") {
                    REQUIRE(0 == lst.total_words());
                    REQUIRE(0 == lst.num_different_words());
                    REQUIRE(0 == lst.num_singletons());
                    REQUIRE(lst.words_in_order().empty());
                }
            }
        }
    }
    SUBCASE("kk Adding a word to an empty WordlistTest") {
        GIVEN("An empty WordlistTest") {
            WordlistTest lst;
            WHEN("We add a word") {
                lst.add_word("hello");
                THEN("There should be one word") {
                    REQUIRE(1 == lst.total_words());
                    REQUIRE(1 == lst.num_different_words());
                    REQUIRE(1 == lst.num_singletons());
                    REQUIRE(1 == lst.get_count("hello"));
                    Heights expected_heights = mk_heights({make_pair(0, 0)});
                    Heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);
                    REQUIRE("hello 1" == lst.most_frequent());
                }
            }
        }
    }
    SUBCASE("Searching for a word not in the WordlistTest") {
        GIVEN("An empty WordlistTest") {
            WordlistTest lst;
            WHEN("We search for a word") {
                THEN("There are no occurrences of the word") {
                    REQUIRE(0 == lst.get_count("hi"));
                }

            }
        }
    } 
    SUBCASE("Adding a word twice") {
        GIVEN("An empty WordlistTest") {
            WordlistTest lst;
            WHEN("Adding the same word twice") {
                lst.add_n({"1", "1"});
                THEN("There is one unique word with two occurences in the list") {
                    REQUIRE(1 == lst.num_different_words());
                    REQUIRE(0 == lst.num_singletons());
                    REQUIRE(2 == lst.total_words());

                    Heights expected_heights = mk_heights({make_pair(0, 0)});
                    Heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);
                    string expected_most_frequent("1 2");
                    REQUIRE(expected_most_frequent == lst.most_frequent());
                }
            }
        }
    }
    SUBCASE("Rebalancing a tree") {
        GIVEN("An empty WordlistTest") {
            WordlistTest lst;
            WHEN("Adding a collection of words") {
                lst.add_n({"c", "b", "a"});
                THEN("The WordlistTest will have the following inorder traversal: [a, b, c]") {
                    REQUIRE(3 == lst.num_singletons());
                    REQUIRE(3 == lst.num_different_words());
                    REQUIRE(3 == lst.total_words());

                    vector<string> actual = lst.words_in_order();
                    vector<string> expected{"a", "b", "c"};
                    REQUIRE(expected == actual);

                    Heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0)});
                    Heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);

                    string expected_most_frequent("a 1");
                    REQUIRE(expected_most_frequent == lst.most_frequent());
                }
            }
        }
    }
    SUBCASE("Rebalancing a tree with a large left subtree") {
        GIVEN("An empty WordlistTest") {
            WordlistTest lst;
            WHEN("Adding a collection of words")
            {
                lst.add_n({"q", "r", "c", "b", "a"});
                THEN("The WordlistTest will have the following inorder traversal: [a, b, c, q, r]")
                {
                    REQUIRE(5 == lst.num_singletons());
                    REQUIRE(5 == lst.num_different_words());
                    REQUIRE(5 == lst.total_words());

                    vector<string> actual = lst.words_in_order();
                    vector<string> expected{"a", "b", "c", "q", "r"};
                    REQUIRE(expected == actual);

                    Heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0), make_pair(2, 1), make_pair(0,0)});
                    Heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);

                    string expected_most_frequent("a 1");
                    REQUIRE(expected_most_frequent == lst.most_frequent());
                }
            }
        }
    }
    SUBCASE("Right rotation") {
        GIVEN("An unbalanced tree with a larger right subtree") {
            WordlistTest lst;
            lst.add_n({"a", "b", "c"});
            WHEN("Balancing the list") {
                THEN("The tree is balanced") {
                    Words expected{"a", "b", "c"};
                    REQUIRE(expected == lst.words_in_order());
                    Heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0)});
                    Heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);
                }
            }
        }
    }
    SUBCASE("Rotating a larger unbalanced tree") {
        GIVEN("An unbalanced tree with a larger right subtree") {
            WordlistTest lst;
            lst.add_n({"c", "b", "r", "q", "s"});

            Words expected{"b", "c", "q", "r", "s"};
            REQUIRE(expected == lst.words_in_order());

            Heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 2), make_pair(0, 0), make_pair(1, 1), make_pair(0, 0)});
            Heights actual_heights = lst.all_heights();
            REQUIRE(expected_heights == actual_heights);

            WHEN("Balancing the list") {
                lst.add_word("t");
                THEN("The tree is balanced") {
                    Words expected{"b", "c", "q", "r", "s", "t"};
                    Heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0), make_pair(2, 2), make_pair(0, 1), make_pair(0, 0)});
                    Heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);
                    REQUIRE(expected == lst.words_in_order());
                }
            }
        }
    }
    SUBCASE("Rebalancing a tree with a large right subtree") {
        GIVEN("An empty WordlistTest") {
            WordlistTest lst;
            WHEN("Adding a collection of words")
            {
                lst.add_n({"q", "r", "c", "b", "a"});
                THEN("The WordlistTest will have the following inorder traversal: [a, b, c, q, r]")
                {
                    REQUIRE(5 == lst.num_singletons());
                    REQUIRE(5 == lst.num_different_words());
                    REQUIRE(5 == lst.total_words());

                    vector<string> actual = lst.words_in_order();
                    vector<string> expected{"a", "b", "c", "q", "r"};
                    REQUIRE(expected == actual);

                    Heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0), make_pair(2, 1), make_pair(0,0)});
                    Heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);

                    string expected_most_frequent("a 1");
                    REQUIRE(expected_most_frequent == lst.most_frequent());
                }
            }
        }
}
    SUBCASE("Right left rotation") {
        GIVEN("A tree which can be balanced by a right left rotation") {
            WordlistTest lst;
            lst.add_n({"a", "d", "c"});
            WHEN("Balancing the tree") {
                THEN("The tree is balanced") {
                    Words expected{"a", "c", "d"};
                    REQUIRE(expected == lst.words_in_order());
                    Heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0)});
                    Heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);

                    string expected_most_frequent("a 1");
                    REQUIRE(expected_most_frequent == lst.most_frequent());

                    REQUIRE(3 == lst.total_words());
                    REQUIRE(3 == lst.num_different_words());
                    REQUIRE(3 == lst.num_singletons());
                }
            }
        }
    }
    SUBCASE("Right left rotation on a larger tree") {
        GIVEN("An empty Wordlist") {
            WordlistTest lst;
            WHEN("Adding a collection of words") {
                lst.add_n({"e", "b", "a", "h", "f", "l", "o", "m", "m"});
                THEN("The tree is balanced") {
                    Words expected_words{"a", "b", "e", "f", "h", "l", "m", "o"};
                    REQUIRE(expected_words == lst.words_in_order());

                    Heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0), make_pair(2, 3), make_pair(0, 0), make_pair(1, 2), make_pair(0, 0), make_pair(1, 0)});
                    Heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);

                    REQUIRE("m 2" == lst.most_frequent());

                    REQUIRE(9 == lst.total_words());
                    REQUIRE(8 == lst.num_different_words());
                    REQUIRE(7 == lst.num_singletons());
                }
            }
        }
    }
}



TEST_CASE("left_right_rotation") {
    SUBCASE("A small tree") {
        GIVEN("A small unbalanced tree") {
            WordlistTest lst;
            lst.add_n({"e", "b"});

            REQUIRE(Words{"b", "e"} == lst.words_in_order());

            Heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 0)});
            Heights actual_heights = lst.all_heights();
            REQUIRE(expected_heights == actual_heights);

            WHEN("Balancing the tree") {
                lst.add_word("c");
                THEN("The tree is balanced") {
                    Words expected_words{"b", "c", "e"};
                    REQUIRE(expected_words == lst.words_in_order());
                    Heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0)});
                    Heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);

                }
            }
        }
    }
    SUBCASE("A larger tree") {
        GIVEN("A larger unbalanced tree") {
            WordlistTest lst;
            lst.add_n({"q", "r", "h", "a"});
            Words expected_words{"a", "h", "q", "r"};
            REQUIRE(expected_words == lst.words_in_order());

            Heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 0), make_pair(2, 1), make_pair(0, 0)});
            Heights actual_heights = lst.all_heights();
            REQUIRE(expected_heights == actual_heights);
            WHEN("Balancing the tree") {
                lst.add_word("b");
                THEN("The tree is balanced") {
                    Words expected_words{"a", "b", "h", "q", "r"};
                    REQUIRE(expected_words == lst.words_in_order());

                    Heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0), make_pair(2, 1), make_pair(0, 0)});
                    Heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);
                }
            }
        }
    }
}

TEST_CASE("data") {
    SUBCASE("A small list") {
        GIVEN("A small list of words") {
            // WordlistTest lst = WordlistTest("small.txt");
            WordlistTest lst;
            string word;
            ifstream file("small.txt");
            // Add the words to the list while there is input to process

            while (file >> word)
            {
                cout << "-- Word: " << "$" <<  word  << "$" << endl;
                lst.add_word(word);
                // lst.add_word("is");
                // lst.add_word("a");


                auto words = lst.words_in_order();
                for_each(words.begin(), words.end(), [](string word) {cout << word << ", ";});
                cout << endl;
            }

            file.close();
            WHEN("Getting the information about the list") {
                ListData actual = lst.data();
                THEN("It contains the data about the list") {
                    vector<string> actual_words = lst.words_in_order();
                    vector<string> expected_words{"This", "a", "is", "or", "test", "test?", "this"};
                    REQUIRE(expected_words == actual_words);
                    REQUIRE(9 == lst.total_words());

                    ListData expected{7, 9, "a 2", 5, 71};
                    REQUIRE(expected == actual);
                }
                
            }
        }
    }
}