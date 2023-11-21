#include "Wordlist.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("WordlistTest") {
    SUBCASE("Empty WordlistTest has no words") {
        GIVEN("An empty WordlistTest") {
            WordlistTest lst;
            WHEN("We check the number of words") {
                THEN("There should be none") {
                    REQUIRE(0 == lst.total_words());
                    REQUIRE(0 == lst.num_different_words());
                    REQUIRE(0 == lst.num_singletons());
                }
            }
        }
    }
    SUBCASE("Adding a word to an empty WordlistTest") {
        GIVEN("An empty WordlistTest") {
            WordlistTest lst;
            WHEN("We add a word") {
                lst.add_word("hello");
                THEN("There should be one word") {
                    REQUIRE(1 == lst.total_words());
                    REQUIRE(1 == lst.num_different_words());
                    REQUIRE(1 == lst.num_singletons());
                    REQUIRE(1 == lst.get_count("hello"));
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
                lst.add_word("1");
                lst.add_word("1");
                THEN("There is one unique word with two occurences in the list") {
                    REQUIRE(1 == lst.num_different_words());
                    REQUIRE(0 == lst.num_singletons());
                    REQUIRE(2 == lst.total_words());
                }
            }
        }
    }
    // SUBCASE("Rebalancing a tree") {
    //     GIVEN("An empty WordlistTest") {
    //         WordlistTest lst;
    //         WHEN("Adding a collection of words") {
    //             lst.add_word("c");
    //             lst.add_word("b");
    //             lst.add_word("a");
    //             THEN("The WordlistTest will have the following inorder traversal: [a, b, c]") {
    //                 vector<string>& actual = inorder_traversal(lst);
    //                 vector<string> expected{"a", "b", "c"};
    //                 REQUIRE(expected == actual);
    //             }
    //         }
    //     }
    // }
}

TEST_CASE("is_imbalanced_left") {
    SUBCASE("A node with no children") {
        GIVEN("A childless node") {
           WordlistTest lst;
           lst.add_word("a");
           WHEN("Calling the function") {
            THEN("It returns false") {
                REQUIRE_FALSE(lst.is_imbalanced_on_left(lst.root));
            }
           }
        }
    }
    SUBCASE ("A node with one child") {
        GIVEN("A node with one child") {
           WordlistTest lst;
           lst.add_word("a");
           lst.add_word("b");
           WHEN("Calling the function") {
            THEN("It returns false") {
                REQUIRE_FALSE(lst.is_imbalanced_on_left(lst.root));
            }
           }
        }
    }
    // SUBCASE ("A node with a left and right children") {
    //     GIVEN("A node with a child in the left and right subtree") {
    //        WordlistTest lst;
    //        lst.add_word("c");
    //        lst.add_word("d");
    //        lst.add_word("a");
    //        WHEN("Calling the function") {
    //         THEN("It returns false") {
    //             REQUIRE(1 == lst.root->left_height);
    //             REQUIRE(1 == lst.root->right_height);
    //             REQUIRE_FALSE(lst.is_imbalanced_on_left(lst.root));
    //         }
    //        }
    //     }
    // }
}