#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "test.h"
#include "a4_base.h"
#include "a4_sort_implementations.h"
#include <string>
#include <sstream>
#include <list>
#include <algorithm>  
#include "doctest.h"
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

template<> struct StringMaker<SwapLocations> {
  static String convert(const SwapLocations& locs) {
    ostringstream oss;
    oss << "[";
    oss << locs.small << ", " << locs.large << "]";
    return oss.str().c_str();
  }
};

};  // namespace doctest


TEST_CASE("rand_num") {
  SUBCASE(
      "Generating a random number from the range [a, a] returns the value a") {
    GIVEN("A range of values [2, 2]") {
      WHEN("Generating a random number in that range") {
        int actual = rand_num(2, 2);
        THEN("The number outputted should equal two") { REQUIRE(2 == actual); }
      }
    }
  }
}
TEST_CASE("rand_vec") {
  SUBCASE(
      "Generating a value in the range [a, b] returns a value x which has the "
      "following properties: a <= x <= b") {
    GIVEN("A range of values [1, 100]") {
      WHEN("Generating 100 values in this range") {
        vector<int> actual = rand_vec(100, 1, 100);
        THEN("All the values should lie in between 1 and 100") {
          REQUIRE(all_of(actual.begin(), actual.end(),
                         [](int val) { return 1 <= val && val <= 100; }));
        }
      }
    }
  }
  SUBCASE("Random negative values can be generated") {
    GIVEN("A range of values [-10, -1]")
    WHEN("Generating 100 values in this range") {
      vector<int> actual = rand_vec(100, -10, -1);
      THEN("All the values lie in between -10 and -1") {
        REQUIRE(all_of(actual.begin(), actual.end(),
                       [](int val) { return -10 <= val && val <= -1; }));
      }
    }
  }
  SUBCASE("Random and positive values can be generated") {
    GIVEN("A range of values [-10, 10]") {
        WHEN("Generating 100 values in this range") {
          vector<int> actual = rand_vec(100, -10, 10);
          THEN("All the values lie in between -10 and 10") {
          REQUIRE(all_of(actual.begin(), actual.end(),
                         [](int val) { return -10 <= val && val <= 10; }));
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
                insert_sort_order(coll,0, 3, info);
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

TEST_CASE("Insertion sort") {
    SUBCASE("Sorting a collection of numbers") {
        GIVEN("A collection of numbers") {
            vector<int> coll = rand_vec(100, -100, 100);
            vector<int> expected = coll;
            WHEN("Ordering the collection") {
                insertion_sort(coll);
                THEN("The collection will be in ascending order") {
                    sort(expected.begin(), expected.end());
                    REQUIRE(expected == coll);
                }
            }
        }
    }
}
TEST_CASE("add_remaining_items") {
    SUBCASE("Adding items using an invalid range does nothing to the list being added to") {
        GIVEN("An initial collection and a nonempty second collection") {
            vector<int> coll;
            vector<int> src{1, 2};
            WHEN("Copying elements from the second collection to the first using an invalid range") {
                add_remaining_items(coll, src, 1, 0);
                THEN("The first collection remains unchanged") {
                    REQUIRE(coll.empty());
                }
            }
        }
    }
    SUBCASE("Adding items using a valid range copies the elements from the second collection onto the first") {
        GIVEN("An initial empty collection and a nonempty second collection") {
            vector<int> coll;
            vector<int> src{1, 2};
            WHEN("Copying all the elements from the second collection onto the first") {
                add_remaining_items(coll, src, 0, 1);
                THEN("The first collection should only have the items in the second collection") {
                    REQUIRE(src == coll);
                }
            }
        }
    }
}

TEST_CASE("overwrite_coll") {
    SUBCASE(
        "Overwriting a range of items in the source collection with the items "
        "of a second collection replaces the items in the first collection "
        "contained in the range with the items of the second collection") {
        GIVEN(
            "A nonempty collection to overwrite and a collection to copy from") {
            vector<int> overwrite{1, 2, 3};
            vector<int> cpy_from{4, 5, 6};
            WHEN("Overwriting the first collection with the elements of the second collection") {
                THEN("The first and second collection will have the same elements") {
                    REQUIRE(overwrite_coll(overwrite, cpy_from, 0) == cpy_from);
                }
            }
        }
    }
}

TEST_CASE("merge") {
    SUBCASE("Ordering an ordered collection returns the same collection") {
        GIVEN("An ordered collection") {
            vector<int> coll{1,2, 3, 4};
            WHEN("Ordering the collection") {
                THEN("The collection will remain unchanged") {
                    vector<int> expected{1, 2, 3, 4};
                    REQUIRE(expected == merge(coll, 0, 2, 3));
                }
            }
        }
    }
    SUBCASE("Ordering a partially unordered collection returns the collection in ascending order") {
        GIVEN("A partially unordered collection") {
            vector<int> coll{4, 5, 1, 2};
            WHEN("Ordering the collection") {
                THEN("The collection is in ascending order") {
                    vector<int> expected{1, 2, 4, 5};
                    REQUIRE(expected == merge(coll, 0, 2, 3));
                }
            }
        }
    }
}

// TEST_CASE("find_smaller_than") {
//     SUBCASE("finding a smaller element in a collection where the pivot is the greatest element returns a value") {
//         GIVEN("A collection where the pivot is the greatest element") {
//             vector<int> coll{1, 2, 3, -4};
//             WHEN("Finding an element smaller than the pivot") {
//                 int actual = find_smaller_than(coll, 3, 0, 3);
//                 THEN("The smallest element found is the last element") {
//                     REQUIRE(3 == actual);
//                 }
//             }
//         }
//     }
// }

// TEST_CASE("find_larger_than") {
//     SUBCASE("finding a smaller element in a collection where the pivot is the greatest element returns a value") {
//         GIVEN("A collection where the pivot is the greatest element") {
//             vector<int> coll{1, 2, 3, -4};
//             WHEN("Finding an element smaller than the pivot") {
//                 int actual = find_larger_than(coll, 0, 3, 3);
//                 THEN("The smallest element found is the last element") {
//                     REQUIRE(-1 == actual);
//                 }
//             }
//         }
//     }
// }

TEST_CASE("find_swap_pair") {
    SUBCASE(
        "Finding the position of the items to swap when there is no "
        "item larger than the pivot returns an invalid index for the "
        "larger element") {
        GIVEN(
            "A collection with a pivot that is larger than any other element") {
            vector<int> coll{1, 2, 3, -4};
            Sort_stats info;
            WHEN("Finding the elements to swap") {
                SwapLocations actual = find_swap_pair(coll, 0, 3, 2, info);
                THEN("The index of the larger element is invalid") {
                    SwapLocations expected(3, -1);
                    REQUIRE(expected == actual);
                }
            }
        }
    }
    SUBCASE(
        "Finding the positions of the items to swap when there is no item "
        "smaller than the pivot returns an invalid index for the smaller "
        "element") {
        GIVEN("A collection with no elements smaller than the pivot") {
            vector<int> coll{5, 6, 3, 5};
            Sort_stats info;
            WHEN("Finding the next elements to swap") {
                SwapLocations actual = find_swap_pair(coll, 0, 3, 2, info);
                THEN("The index of the smaller index is invalid") {
                    SwapLocations expected(-1, 0);
                    REQUIRE(expected == actual);
                }
            }
        }
    }
    SUBCASE(
        "Finding the positions of the next items to swap when there are "
        "elements smaller and bigger than the pivot returns valid indexes for "
        "both locations") {
            GIVEN("A collection with elements bigger and smaller than the pivot") {
                vector<int> coll{1, 4, 3, -1, 5};
                Sort_stats info;
                WHEN("Finding the next elements to swap") {
                    SwapLocations actual = find_swap_pair(coll, 0, 4, 2, info);
                    THEN("The index of both locations are valid") {
                        SwapLocations expected(3, 1);
                        REQUIRE(expected == actual);
                    }
                }
            }
        }
}

TEST_CASE("can_swap") {
    SUBCASE("Locations where one of the positions is invalid are valid locations") {
            SUBCASE(
                "A location where the smallest element has an invalid index is "
                "a valid location") {
                GIVEN(
                    "A location where the smallest element has an invalid "
                    "index") {
                    SwapLocations locs(-1, 4);
                    WHEN("Checking if the locations are valid") {
                        THEN("The locations are deemed valid") {
                          REQUIRE(can_swap(locs));
                        }
                    }
                }
            }
            SUBCASE(
                "A location where the largest element has an invalid index is "
                "a valid location") {
                GIVEN(
                    "A location where the largest element has an invalid "
                    "index") {
                    SwapLocations locs(1, -1);
                    WHEN("Checking if the locations are valid") {
                        THEN("The location is deemed valid") {
                          REQUIRE(can_swap(locs));
                        }
                    }
                }
            }
    }
    SUBCASE("Locations where the positions overlap are valid locations") {
            GIVEN("A location where the index of the smallest element is smaller than the bigger element") {
                SwapLocations locs(0, 2);
                WHEN("Checking if the locations are valid") {
                    THEN("The location is deemed valid") {
                        REQUIRE(can_swap(locs));
                    }
                }
            }
    }
    SUBCASE("Locations where both positions are invalid are invalid positions") {
        GIVEN("A location where the indexes of both elements are invalid") {
            SwapLocations loc(-1, -1);
            WHEN("Checking if the location is valid") {
                THEN("The location is deemed valid") {
                    REQUIRE_FALSE(can_swap(loc));
                }
            }
        }
    }
}

TEST_CASE("merge_sort") {
    SUBCASE("Ordering a random list of integers returns a list in ascending order") {
        GIVEN("A collection of random integers") {
            vector<int> coll = rand_vec(100, -100, 100);
            WHEN("Sorting the collection") {
                THEN("The collection is in ascending order") {
                    vector<int> expected{coll};
                    sort(expected.begin(), expected.end());
                    merge_sort(coll);
                    REQUIRE(expected == coll);
                }
            }
        }
    }
}

TEST_CASE("quick_sort") {
    SUBCASE("Ordering a random list of numbers returns the list in ascending order") {
        GIVEN("A collection of random numbers") {
            vector<int> coll = rand_vec(100, -100, 100);
            WHEN("Sorting the collection") {
                THEN("The collection is in ascending order") {
                    vector<int> expected{coll};
                    sort(expected.begin(), expected.end());
                    quick_sort(coll);
                    REQUIRE(expected == coll);
                }
            }
        }
    }
}

TEST_CASE("is_pair_within_coll") {
    SUBCASE(
        "A pair where the second element is within the collection is a valid "
        "pair") {
        GIVEN(
            "The location of the second element which is inside the "
            "collection") {
            int pos = 2;
            WHEN("Checking if the location is inside the collection") {
                THEN("The location will be deemed valid") {
                    REQUIRE(is_pair_within_coll(pos, 3));
                }
            }
        }
    }
    SUBCASE(
        "A pair where the second element is outside the collection is an "
        "invalid pair") {
        GIVEN(
            "The location of the second element which is outside the "
            "collection") {
            int pos = 2;
            WHEN("Checking if the location is inside the collection") {
                THEN("The location will be deemed invalid") {
                    REQUIRE_FALSE(is_pair_within_coll(pos, 1));
                }
            }
        }
    }
}
TEST_CASE("order_elems_by_gap") {
    SUBCASE("Ordering the elements with a gap bigger than the size of the collection leaves the collection as it was") {
        GIVEN("A collection and a gap bigger than the size of the collection") {
            vector<int> coll{-2, -12, 1};
            int gap = 4;
            Sort_stats info;
            WHEN("Ordering the elements using the gap") {
                order_elems_by_gap(coll, gap, info);
                THEN("The collection remains unchanged") {
                    vector<int> expected{-2, -12, 1};
                    REQUIRE(expected == coll);
                }
            }
        }
    }
    SUBCASE("Ordering the elements with a gap smaller than the size of the collection leaves the collection partially ordered") {
        GIVEN("A collection with a gap smaller than the size of the collection") {
            vector<int> coll{13, -1, -4, -29};
            int gap = 2;
            Sort_stats info;
            WHEN("Ordering the elements using the gap") {
                order_elems_by_gap(coll, gap, info);
                THEN("The collection will have some elements swapped around") {
                    vector<int> expected{-4, -29, 13, -1};
                    REQUIRE(expected == coll);
                }
            }
        }
    }
}

TEST_CASE("shell_sort") {
    SUBCASE("Ordering a random list of numbers returns the list in ascending order") {
        GIVEN("A collection of random numbers") {
            vector<int> coll = rand_vec(100, -100, 100);
            WHEN("Sorting the collection") {
                THEN("The collection is in ascending order") {
                    vector<int> expected{coll};
                    sort(expected.begin(), expected.end());
                    shell_sort(coll);
                    REQUIRE(expected == coll);
                }
            }
        }
    }
}


TEST_CASE("iquick_sort") {
    SUBCASE("Ordering a random list of numbers returns the list in ascending order") {
        GIVEN("A collection of random numbers") {
            vector<int> coll = rand_vec(100, -100, 100);
            WHEN("Sorting the collection") {
                THEN("The collection is in ascending order") {
                    vector<int> expected{coll};
                    sort(expected.begin(), expected.end());
                    iquick_sort(coll);
                    REQUIRE(expected == coll);
                }
            }
        }
    }
}

TEST_CASE("Heap") {
    SUBCASE("An new heap has no elements") {
        GIVEN("A new heap") {
            Heap<int> heap;
            THEN("It will be empty") {
                REQUIRE(heap.empty());
            }
        }
    }
}

TEST_CASE("parent") {
    SUBCASE("The root has no parent") {
        GIVEN("The position of the root") {
            WHEN("Finding the parent of the root") {
                THEN("No parent is found") {
                    REQUIRE(-1 == parent(0));
                }
            }
        }
    }
}