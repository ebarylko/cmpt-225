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
                    REQUIRE(actual == 2);
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