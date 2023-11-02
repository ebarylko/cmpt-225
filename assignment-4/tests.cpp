#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "a4_base.h"
#include "a4_sort_implementations.h"
#include "test.h"
#include <cassert>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>  
// #include "rapidcheck.h"

using namespace std;

namespace doctest {
template <typename T>
struct StringMaker<vector<T>> {
  static String convert(const vector<T>& coll) {
    ostringstream oss;

    oss << "[";
    // NOLINTNEXTLINE(*-use-auto)
    for (typename std::vector<T>::const_iterator it = coll.begin();
         it != coll.end();) {
      oss << *it;
      if (++it != coll.end()) {
        oss << ", ";
      }
    }
    oss << "]";
    return oss.str().c_str();
  }
};

template<> struct StringMaker<NextElems> {
  static String convert(const NextElems& coll) {
    ostringstream oss;
    oss << "[";
    oss << coll.fst << ", " << coll.snd << "]";
    return oss.str().c_str();
  }
};
};  // namespace doctest

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

TEST_CASE("swap") {
    SUBCASE("Swapping two elements in a collection changes their positions") {
        GIVEN("A collection of elements") {
            vector<int> coll{1, 2, 3};
            WHEN("Swapping the first and last element") {
               swap(coll, 0, 2);
                THEN("The first and last element in the collection will interchange positions") {
                    vector<int> expected{3, 2, 1};
                    REQUIRE(expected == coll);
                }
            }
        }
    }
}

bool no_comparisons(const Sort_stats& info) {
    return info.num_comparisons == 0;
}

void print_vec(const vector<int>& coll) {
    for(auto curr = coll.begin(); curr != coll.end(); curr++) {
        cout << *curr << endl;
    }
}

TEST_CASE("bubble swap") {
    SUBCASE("Moving the largest element to the end in an empty collection does nothing") {
        GIVEN("An empty collection") {
            vector<int> coll;
            Sort_stats info;
            WHEN("Moving the largest element to the end") {
                bubble_swap(coll, 0, info);
                THEN("Nothing occurs to the collection") {
                    REQUIRE(coll.empty());
                    REQUIRE(no_comparisons(info));
                }
            }
        }
    }
    SUBCASE(
        "Moving the largest element to the end in a nonempty collection "
        "returns a collection with the same amount of elements and with the "
        "largest element placed at the end") {
            GIVEN("A nonempty collection") {
                vector<int> coll{5, 1, 2, 3}; 
                Sort_stats info;
                WHEN("Moving the largest element to the end") {
                    bubble_swap(coll, 3, info);
                    THEN("The largest element should be at the end") {
                        vector<int> expected{1, 2, 3, 5};
                        REQUIRE(expected == coll);
                    }
                }
        }
}
SUBCASE("Moving the largest element to the end in a sorted collection does not change the collection") {
    GIVEN("An ordered collection") {
        vector<int> coll{1, 2, 3};
        Sort_stats info;
        WHEN("Moving the largest element to the end") {
            bubble_swap(coll, 2, info);
            THEN("The collection should remain unchanged") {
                vector<int> expected{1, 2, 3};
                REQUIRE(expected == coll);
            }
        }
    }
}
}

TEST_CASE("insert_sort_order") {
    SUBCASE("When ordering an element in a collection, the element is moved to its correct location") {
        GIVEN("A nonempty collection") {
            vector<int> coll{1, 2, 3, -1};
            Sort_stats info;
            WHEN("Ordering the last element") {
                insert_sort_order(coll, 3, info);
                THEN("The collection should be in ascending order") {
                    vector<int> expected{-1, 1, 2, 3};
                    REQUIRE(expected == coll);
                }
            }
        }
    }
}

TEST_CASE("add_smallest_elem") {
    SUBCASE("The smaller of the two items in a collection is added to the end of another collection") {
        GIVEN("A nonempty unordered collection and an empty second collection") {
            vector<int> coll{2, 1};
            vector<int> sorted;
            WHEN("Adding the smallest element from the first collection onto the second") {
                add_smallest_elem(sorted, coll, 0, 1);
                THEN("The smallest element should appear at the end of the second collection") {
                    vector<int> expected{1};
                    REQUIRE(expected == sorted);
                }
            }
        }
    }
    SUBCASE("Adding an item returns the updated positions of the next items to check") {
        GIVEN("A nonempty collection of unordered items and a second collection") {
            vector<int> coll{2, 1};
            vector<int> sorted;
            WHEN("Addng the smallest elem from the first to the second collection") {
                NextElems actual = add_smallest_elem(sorted, coll, 0, 1);
                THEN("The position of the next items to check will include an invalid position for the second location") {
                    NextElems expected{0, 2};
                    REQUIRE(expected == actual);
                }
            }
        }
    }
    SUBCASE("Adding an item from the first location updates the position of the first location") {
        GIVEN("A nonempty collection of unordered items and a second collection") {
            vector<int> coll{1, 2};
            vector<int> sorted;
            WHEN("Addng the first element from the unordered collection to the ordered one") {
                NextElems actual = add_smallest_elem(sorted, coll, 0, 1);
                THEN("The location of the first item will be incremented by one") {
                    NextElems expected{1, 1};
                    REQUIRE(expected == actual);
                }
            }
        }
    }
}