#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "a4_base.h"
#include "a4_sort_implementations.h"
#include "test.h"
#include <cassert>
#include <string>
#include <algorithm>  

using namespace std;


TEST_CASE("rand_num") {
    SUBCASE("Generating a random number from the range [a, a] returns the value a") {
        GIVEN("A range of values [2, 2]") {
            WHEN("Generating a random number in that range") {
                int actual = rand_num(2, 2);
                THEN("The number outputted should equal two") {
                    REQUIRE(2 == actual);
                }
            }
        }
    }
    SUBCASE("Generating a value in the range [a, b] returns a value x which has the following properties: a <= x <= b") {
        GIVEN( "A range of values [1, 100]" ) {
            WHEN("Generating 100 values in this range") {
                vector<int> actual;
                for(int i = 0; i < 100; i++) {
                    actual.push_back(rand_num(1, 100));
                }
                THEN("All the values should lie in between 1 and 100") {
                    REQUIRE(all_of(actual.begin(), actual.end(), [](int val) {return 1 <= val && val <= 100;}));
                }
            }
        }
    }
}

TEST_CASE("min_elem_pos") {
    SUBCASE("The location of the smallest element in a collection with one element is the first spot") {
        GIVEN("An collection with one element") {
            vector<int> coll{1};
            WHEN("Searching for the position of the smallest element") {
                int pos = min_elem_pos(coll, 0);
                THEN("The index of the smallest element will be zero") {
                    REQUIRE(0 == pos);
                }
            }
        }
    }
    SUBCASE("The location of the smallest element in a collection with unique elements is the position with the smallest value") {
        GIVEN("A collection wit unique values") {
            vector<int> coll{1, 3, -4, 2};
            WHEN("Finding the position of the smallest element") {
                int pos = min_elem_pos(coll, 0);
                THEN("The index of the smallest location should be 2") {
                    REQUIRE(2 == pos);
                }
            }
        }
    }
    SUBCASE("The location of the smallest element in a collection with duplicates is the position of the first instance of the smallest value") {
        GIVEN("A collection with dupllicate values") {
            vector<int> coll{2, 3, 1, 1};
            WHEN("Finding the position of the smallest element") {
                int pos = min_elem_pos(coll, 0);
                THEN("The index of the smallest element is 2") {
                    REQUIRE(2 == pos);
                }
            }
        }
    }
}