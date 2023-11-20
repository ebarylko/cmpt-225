#include "Wordlist.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("Wordlist") {
    SUBCASE("Empty Wordlist has no words") {
        GIVEN("An empty Wordlist") {
            Wordlist lst;
            WHEN("We check the number of words") {
                THEN("There should be none") {
                    REQUIRE(0 == lst.total_words());
                    REQUIRE(0 == lst.num_different_words());
                    REQUIRE(0 == lst.num_singletons());
                }
            }
        }
    }
    SUBCASE("Adding a word to an empty Wordlist") {
        GIVEN("An empty Wordlist") {
            Wordlist lst;
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
    SUBCASE("Searching for a word not in the Wordlist") {
        GIVEN("An empty Wordlist") {
            Wordlist lst;
            WHEN("We search for a word") {
                THEN("There are no occurrences of the word") {
                    REQUIRE(0 == lst.get_count("hi"));
                }

            }
        }
    } 
    SUBCASE("Adding a word twice") {
        GIVEN("An empty Wordlist") {
            Wordlist lst;
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
    // SUBCASE("Adding two elements to an empty Wordlist") {
    //     GIVEN("An empty Wordlist") {
    //         Wordlist lst;
    //         WHEN("Adding two words to the list") {
    //             lst.add_word("hi");
    //             lst.add_word("bye");
    //             THEN("Both words will be in the list") {
    //                 REQUIRE(lst.contains("hi"));
    //                 REQUIRE(lst.contains("bye"));
    //                 REQUIRE(2 == lst.num_different_words() == lst.num_singletons() == lst.num_singletons());
    //             }
    //         }
    //     }
    // }
}