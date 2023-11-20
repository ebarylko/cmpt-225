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
                }
            }
        }
    }
}