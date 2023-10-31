#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "a4_base.h"
#include "a4_sort_implementations.h"
#include "test.h"
#include <cassert>
#include <string>

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
}