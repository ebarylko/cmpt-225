#include "Wordlist.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <ostream>

typedef WordlistTest::heights heights;
typedef WordlistTest::subtree_heights node_heights;
template <typename T1, typename T2> 
ostream& operator<<(ostream& os, const pair<T1, T2>& heights) {
    os << "[" << heights.first << ", " << heights.second << "]";
    return os;
}

// ostream& operator<<(ostream& os, const heights& all_heights) {
//     for_each(all_heights.begin(), all_heights.end(), [&os](const node_heights& heights) {
//         os << heights;
//     });
//     return os;
// }

namespace doctest
{

// template <> struct StringMaker<heights> {
//      static string convert(const heights& all_heights) {
//         stringstream os;
//         os << "[";
//         os << all_heights.front().first;
//         // os << all_heights;
//         os << "]";
//         return os.str().c_str();
// }
// };


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
// preguntar papa porque esto no esta funcionando

// template <>
// struct StringMaker<heights>
// {

//     static String convert(const heights& in) {
//         std::ostringstream oss;
//         oss << "[";
//         // NOLINTNEXTLINE(*-use-auto)
//         for (auto it = in.begin(); it != in.end();) {
//             oss << "[" << (*it).first << ", " << it->second << "]";
//             if (++it != in.end()) { oss << ", "; }
//         }
//         oss << "]";
//         return oss.str().c_str();
//     }
// };
}



/**
 * @brief Takes a collection of expected heights and creates a collection of 
 * heights of the nodes visited using inorder traversal
 * 
 * @param all_heights the expected heights
 * @return heights the collection of heights corresponding to the nodes
 * in alphabetical order
 */
heights mk_heights(initializer_list<pair<int,int>> all_heights) {
    heights tree_heights{all_heights};
    return tree_heights;
}

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
                    heights expected_heights = mk_heights({make_pair(0, 0)});
                    heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);
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
                    heights expected_heights = mk_heights({make_pair(0, 0)});
                    heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);
                }
            }
        }
    }
    // fix test
    SUBCASE("Rebalancing a tree") {
        GIVEN("An empty WordlistTest") {
            WordlistTest lst;
            WHEN("Adding a collection of words") {
                lst.add_word("c");
                lst.add_word("b");
                lst.add_word("a");
                THEN("The WordlistTest will have the following inorder traversal: [a, b, c]") {
                    vector<string> actual = lst.words_in_order();
                    vector<string> expected{"a", "b", "c"};
                    REQUIRE(expected == actual);
                    heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0)});
                    heights actual_heights = lst.all_heights();
                    REQUIRE(expected_heights == actual_heights);
                }
            }
        }
    }
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
    SUBCASE ("A node with a left and right children") {
        GIVEN("A node with a child in the left and right subtree") {
           WordlistTest lst;
           lst.add_n({"c", "d", "a"});
        //    lst.unbalanced_word_insertion("c");
        //    lst.unbalanced_word_insertion("d");
        //    lst.unbalanced_word_insertion("a");
        //    lst.add_word("c");
        //    lst.add_word("d");
        //    lst.add_word("a");
           WHEN("Calling the function") {
            THEN("It returns false") {
                heights actual_heights = lst.all_heights();
                heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0)});
                REQUIRE(expected_heights == actual_heights);
                REQUIRE_FALSE(lst.is_imbalanced_on_left(lst.root));
            }
           }
        }
    }
    SUBCASE ("A node with a taller left subtree") {
        GIVEN("A node with a taller left subtree") {
           WordlistTest lst;
           lst.unbalanced_word_insertion("c");
           lst.unbalanced_word_insertion("b");
           lst.unbalanced_word_insertion("a");
           WHEN("Calling the function") {
            THEN("It returns true") {
                heights actual_heights = lst.all_heights();
                heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 0), make_pair(2, 0)});
                REQUIRE(expected_heights == actual_heights);
                REQUIRE(lst.is_imbalanced_on_left(lst.root));
            }
           }
        }
    }
    SUBCASE ("A node with a taller right subtree") {
        GIVEN("A node with a taller right subtree") {
           WordlistTest lst;
           lst.unbalanced_add_n({"c", "d", "e"});
           WHEN("Calling the function") {
            THEN("It returns true") {
                heights actual_heights = lst.all_heights();
                heights expected_heights = mk_heights({make_pair(0, 2), make_pair(0, 1), make_pair(0, 0)});
                REQUIRE(expected_heights == actual_heights);
                REQUIRE_FALSE(lst.is_imbalanced_on_left(lst.root));
            }
           }
        }
    }
}

typedef vector<string> words;

TEST_CASE("find_word") {
    SUBCASE("Finding a word not in the list") {
        GIVEN("An empty wordlist") {
            WordlistTest lst;
            WHEN("Searching for a word") {
                WordlistTest::Node* actual = lst.find_word("hi");
                THEN("No word is found") {
                    REQUIRE_FALSE(actual);
                }
            }
        }

    }
    SUBCASE("Finding a word in the list") {
        GIVEN("A non-empty wordlist") {
            WordlistTest lst;
            lst.add_word("a");
            WHEN("Searching for 'a'") {
                WordlistTest::Node* actual = lst.find_word("a");
                THEN("The word is found") {
                    REQUIRE("a" == actual->word);
                }
            }
        }

    }
    SUBCASE("Finding a word in the right subtree") {
        GIVEN("A Wordlist with a right subtree") {
            WordlistTest lst;
            lst.add_n({"c", "d", "e"});
            WHEN("Searching for 'e'") {
                WordlistTest::Node* actual = lst.find_word("e");
                THEN("The word is found") {
                    words expected_words{"c", "d", "e"};
                    words actual_words = lst.words_in_order();
                    REQUIRE("e" == actual->word);
                    REQUIRE(expected_words == actual_words);
                }
            }
        }

    }
    SUBCASE("Finding a word in the left subtree") {
        GIVEN("A Wordlist with a left subtree") {
            WordlistTest lst;
            lst.add_n({"u", "f", "c", "d"});
            WHEN("Searching for 'd'") {
                WordlistTest::Node* actual = lst.find_word("d");
                THEN("The word is found") {
                    REQUIRE("d" == actual->word);
                    REQUIRE("c" == actual->parent->word);
                }
            }
        }

    }
}

typedef WordlistTest::RotationType Rotation;

TEST_CASE("rotation_type") {
    SUBCASE("left rotation") {
        GIVEN("An unbalanced tree") {
            WordlistTest lst;
            lst.unbalanced_add_n({"d", "c", "b"});
            WHEN("Calling the function") {
                Rotation actual = lst.rotation_type(lst.root);
                Rotation expected = Rotation::left;
                THEN("The rotation needed is a left rotation") {
                    REQUIRE(expected == actual);
                }
            }
        }
    }
    SUBCASE("right rotation") {
        GIVEN("An unbalanced tree") {
            WordlistTest lst;
            lst.unbalanced_add_n({"a", "b", "c"});
            WHEN("Calling the function") {
                Rotation actual = lst.rotation_type(lst.root);
                Rotation expected = Rotation::right;
                THEN("The rotation needed is a left rotation") {
                    REQUIRE(expected == actual);
                }
            }
        }
    }
    SUBCASE("left right rotation") {
        GIVEN("An unbalanced tree") {
            WordlistTest lst;
            lst.add_word("c");
            lst.add_word("a");
            lst.add_word("b");
            WHEN("Calling the function") {
                Rotation actual = lst.rotation_type(lst.root);
                Rotation expected = Rotation::left_right;
                THEN("The rotation needed is a left-right rotation") {
                    REQUIRE(expected == actual);
                }
            }
        }
    }
    SUBCASE("right left rotation") {
        GIVEN("An unbalanced tree") {
            WordlistTest lst;
            lst.add_word("a");
            lst.add_word("d");
            lst.add_word("b");
            WHEN("Calling the function") {
                Rotation actual = lst.rotation_type(lst.root);
                Rotation expected = Rotation::right_left;
                THEN("The rotation needed is a right-left rotation") {
                    REQUIRE(expected == actual);
                }
            }
        }
    }
}


// TEST_CASE("inorder_traversal") {
//     SUBCASE("Empty Wordlist") {
//     GIVEN("An empty Wordlist") {
//         WordlistTest lst;
//         WHEN("Collecting the words in alphabetical order") {
//             THEN("The collection is empty") {
//                 REQUIRE(lst.words_in_order().empty());
//             }
//         }
//     }
//     }
//     SUBCASE("Wordlist with one word") {
//         GIVEN("A Wordlist with one word") {
//             WordlistTest lst;
//             lst.add_word("a");
//             WHEN("Calling the function") {
//                 words actual = lst.words_in_order();
//                 THEN("The only word in the list is 'a'") {
//                     words expected{"a"};
//                     REQUIRE(expected == actual);
//                 }
//             }
//         }
//     }
// }

// TEST_CASE("find_smallest") {
//     SUBCASE("Empty list") {
//         GIVEN("An empty list") {
//             WordlistTest lst;
//             WHEN("Calling the function") {
//                 THEN("It returns null") {
//                     REQUIRE_FALSE(lst.find_smallest(lst.root));
//                 }
//             }
//         }
//     }
//     SUBCASE("Small list") {
//         GIVEN("A list with two elements") {
//             WordlistTest lst;
//             lst.add_word("hi");
//             lst.add_word("a");
//             WHEN("Calling the function") {
//                 THEN("It returns 'a'") {
//                     REQUIRE("a" == lst.find_smallest(lst.root)->word);
//                 }
//             }
//         }
//     }
//     SUBCASE("Tree with a right subtree") {
//         GIVEN("A list with two elements") {
//             WordlistTest lst;
//             lst.add_word("hi");
//             lst.add_word("q");
//             WHEN("Calling the function") {
//                 THEN("It returns 'hi'") {
//                     REQUIRE("hi" == lst.find_smallest(lst.root)->word);
//                 }
//             }
//         }
//     }
// }

// typedef WordlistTest::Node Node;

// TEST_CASE("shift_root") {
//     GIVEN("A root node and a normal node") {
//         WordlistTest lst;
//         lst.add_word("hi");
//         lst.add_word("a");
//         WHEN("Changing the root node from 'hi' to 'a") {
//             lst.shift_root(lst.root->left);
//             THEN("The root is 'a' and contains the information of the previous root") {
//                 vector<int> list_data{2, 2, 2};
//                 words expected{"a"};
//                 REQUIRE(list_data == lst.list_info());
//                 REQUIRE(expected == lst.words_in_order());
//             }
//         }
         
//     }
// }

// TEST_CASE("left-rotation") {
//     SUBCASE("Rotating an unbalanced tree") {
//         GIVEN("An unbalanced tree with a larger left subtree") {
//             WordlistTest lst;
//             lst.add_n({"d", "b", "a"});
//             heights old_heights = lst.all_heights();
//             heights expected_old_heights = mk_heights({make_pair(0, 0), make_pair(1, 0), make_pair(2, 0)});
//             REQUIRE(expected_old_heights == old_heights);
//             WHEN("Balancing the list") {
//                 lst.left_rotation(lst.root);
//                 THEN("The tree is balanced") {
//                     heights actual_heights = lst.all_heights();
//                     heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0)});
//                     words expected{"a", "b", "d"};
//                     REQUIRE(expected == lst.words_in_order());
//                     REQUIRE(expected_heights == actual_heights);
//                 }
//             }
//         }
//     }
//     SUBCASE("Rotating a larger unbalanced tree") {
//         GIVEN("An unbalanced tree with a larger left subtree") {
//             WordlistTest lst;
//             lst.add_n({"q", "z", "h", "e", "i", "a"});
//             WHEN("Balancing the list") {
//                 lst.left_rotation(lst.root);
//                 THEN("The tree is balanced") {
//                     words expected{"a", "e", "h", "i", "q", "z"};
//                     REQUIRE(expected == lst.words_in_order());
//                     heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 0), make_pair(2, 2), make_pair(0, 0), make_pair(1, 1), make_pair(0, 0)});
//                     heights actual_heights = lst.all_heights();
//                     REQUIRE(expected_heights == actual_heights);
//                 }
//             }
//         }
//     }
// }

// TEST_CASE("right-rotation") {
//     SUBCASE("Rotating an unbalanced tree") {
//         GIVEN("An unbalanced tree with a larger right subtree") {
//             WordlistTest lst;
//             lst.add_n({"a", "b", "c"});
//             WHEN("Balancing the list") {
//                 lst.right_rotation(lst.root);
//                 THEN("The tree is balanced") {
//                     words expected{"a", "b", "c"};
//                     REQUIRE(expected == lst.words_in_order());
//                     heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0)});
//                     heights actual_heights = lst.all_heights();
//                     REQUIRE(expected_heights == actual_heights);
//                 }
//             }
//         }
//     }
//     SUBCASE("Rotating a larger unbalanced tree") {
//         GIVEN("An unbalanced tree with a larger right subtree") {
//             WordlistTest lst;
//             lst.add_n({"c", "b", "r", "q", "s", "t"});
//             WHEN("Balancing the list") {
//                 lst.right_rotation(lst.root);
//                 THEN("The tree is balanced") {
//                     words expected{"b", "c", "q", "r", "s", "t"};
//                     heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0), make_pair(2, 2), make_pair(0, 1), make_pair(0, 0)});
//                     heights actual_heights = lst.all_heights();
//                     REQUIRE(expected_heights == actual_heights);
//                     REQUIRE(expected == lst.words_in_order());
//                 }
//             }
//         }
//     }
// }
 
// TEST_CASE("right_left_rotation") {
//     SUBCASE("Rotating a small tree") {
//         GIVEN("A tree which can be balanced by a right left rotation") {
//             WordlistTest lst;
//             lst.add_n({"a", "d", "c"});
//             WHEN("Balancing the tree") {
//                 lst.right_left_rotation(lst.root);
//                 THEN("The tree is balanced") {
//                     words expected{"a", "c", "d"};
//                     REQUIRE(expected == lst.words_in_order());
//                     heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 1), make_pair(0, 0)});
//                     heights actual_heights = lst.all_heights();
//                     REQUIRE(expected_heights == actual_heights);
//                 }
//             }
//         }
//     }
//     SUBCASE("Rotating a larger tree") {
//         GIVEN("A tree which can be balanced by a right left rotation") {
//             WordlistTest lst;
//             lst.add_n({"e", "b", "a", "h", "f", "l", "o", "m"});
//             heights expected_old_heights = mk_heights({make_pair(0, 0), make_pair(1, 0), make_pair(2, 4), make_pair(0, 0), make_pair(1, 3), make_pair(0, 2), make_pair(0, 0), make_pair(1, 0)});
//             heights actual_old_heights = lst.all_heights();
//             REQUIRE(expected_old_heights == actual_old_heights);
//             WHEN("Balancing the tree") {
//                 lst.right_left_rotation(lst.root->right->right);
//                 THEN("The tree is balanced") {
//                     words expected_words{"a", "b", "e", "f", "h", "l", "m", "o"};
//                     REQUIRE(expected_words == lst.words_in_order());
//                     heights expected_heights = mk_heights({make_pair(0, 0), make_pair(1, 0), make_pair(2, 3), make_pair(0, 0), make_pair(1, 2), make_pair(0, 0), make_pair(1, 1), make_pair(0, 0)});
//                     heights actual_heights = lst.all_heights();
//                     REQUIRE(expected_heights == actual_heights);
//                 }
//             }
//         }
//     }
// }

// TEST_CASE("rebalance_tree") {
//     SUBCASE("Rebalance large tree") {
//         GIVEN("A unbalanced tree") {
//             WordlistTest lst;
//             lst.add_n({"e", "h", "f", "l", "o", "m"});
//             WHEN("Rebalancing the tree from the last node added") {
//                 THEN("The tree will be balanced") {
//                     lst.rebalance_tree(lst.root->right->right->right->left);
//                     heights expected_old_heights = mk_heights({make_pair(0, 4), make_pair(0, 0), make_pair(1, 3), make_pair(0, 2), make_pair(0, 0), make_pair(1, 0)});
//                     heights actual_old_heights = lst.all_heights();
//                     REQUIRE(expected_old_heights == actual_old_heights);
//                 }
//             }
//         }
//     }
// }