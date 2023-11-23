#include "Wordlist.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

namespace doctest
{
template <typename T>
struct StringMaker<std::vector<T>>
{
    static String convert(const std::vector<T>& in) {
        std::ostringstream oss;

        oss << "[";
        // NOLINTNEXTLINE(*-use-auto)
        for (typename std::vector<T>::const_iterator it = in.begin(); it != in.end();) {
            oss << *it;
            if (++it != in.end()) { oss << ", "; }
        }
        oss << "]";
        return oss.str().c_str();
    }
};
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
    SUBCASE ("A node with a left and right children") {
        GIVEN("A node with a child in the left and right subtree") {
           WordlistTest lst;
           lst.add_word("c");
           lst.add_word("d");
           lst.add_word("a");
           WHEN("Calling the function") {
            THEN("It returns false") {
                REQUIRE(1 == lst.root->left_height);
                REQUIRE(1 == lst.root->right_height);
                REQUIRE_FALSE(lst.is_imbalanced_on_left(lst.root));
            }
           }
        }
    }
    SUBCASE ("A node with a taller left subtree") {
        GIVEN("A node with a taller left subtree") {
           WordlistTest lst;
           lst.add_word("c");
           lst.add_word("b");
           lst.add_word("a");
           WHEN("Calling the function") {
            THEN("It returns true") {
                REQUIRE(2 == lst.root->left_height);
                REQUIRE(0 == lst.root->right_height);
                REQUIRE(lst.is_imbalanced_on_left(lst.root));
            }
           }
        }
    }
    SUBCASE ("A node with a taller right subtree") {
        GIVEN("A node with a taller right subtree") {
           WordlistTest lst;
           lst.add_word("c");
           lst.add_word("d");
           lst.add_word("e");
           WHEN("Calling the function") {
            THEN("It returns true") {
                REQUIRE(2 == lst.root->right_height);
                REQUIRE(0 == lst.root->left_height);
                REQUIRE_FALSE(lst.is_imbalanced_on_left(lst.root));
            }
           }
        }
    }
}

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
            lst.add_word("c");
            lst.add_word("d");
            lst.add_word("e");
            WHEN("Searching for 'e'") {
                WordlistTest::Node* actual = lst.find_word("e");
                THEN("The word is found") {
                    REQUIRE("e" == actual->word);
                    REQUIRE("d" == actual->parent->word);
                }
            }
        }

    }
    SUBCASE("Finding a word in the left subtree") {
        GIVEN("A Wordlist with a left subtree") {
            WordlistTest lst;
            lst.add_word("u");
            lst.add_word("f");
            lst.add_word("c");
            lst.add_word("d");
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
            lst.add_word("d");
            lst.add_word("c");
            lst.add_word("b");
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
            lst.add_word("a");
            lst.add_word("b");
            lst.add_word("c");
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

typedef vector<string> words;

TEST_CASE("inorder_traversal") {
    SUBCASE("Empty Wordlist") {
    GIVEN("An empty Wordlist") {
        WordlistTest lst;
        WHEN("Collecting the words in alphabetical order") {
            THEN("The collection is empty") {
                REQUIRE(lst.words_in_order().empty());
            }
        }
    }
    }
    SUBCASE("Wordlist with one word") {
        GIVEN("A Wordlist with one word") {
            WordlistTest lst;
            lst.add_word("a");
            WHEN("Calling the function") {
                words actual = lst.words_in_order();
                THEN("The only word in the list is 'a'") {
                    words expected{"a"};
                    REQUIRE(expected == actual);
                }
            }
        }
    }
}

TEST_CASE("find_smallest") {
    SUBCASE("Empty list") {
        GIVEN("An empty list") {
            WordlistTest lst;
            WHEN("Calling the function") {
                THEN("It returns null") {
                    REQUIRE_FALSE(lst.find_smallest(lst.root));
                }
            }
        }
    }
    SUBCASE("Small list") {
        GIVEN("A list with two elements") {
            WordlistTest lst;
            lst.add_word("hi");
            lst.add_word("a");
            WHEN("Calling the function") {
                THEN("It returns 'a'") {
                    REQUIRE("a" == lst.find_smallest(lst.root)->word);
                }
            }
        }
    }
    SUBCASE("Tree with a right subtree") {
        GIVEN("A list with two elements") {
            WordlistTest lst;
            lst.add_word("hi");
            lst.add_word("q");
            WHEN("Calling the function") {
                THEN("It returns 'hi'") {
                    REQUIRE("hi" == lst.find_smallest(lst.root)->word);
                }
            }
        }
    }
}

typedef WordlistTest::Node Node;

TEST_CASE("shift_root") {
    GIVEN("A root node and a normal node") {
        WordlistTest lst;
        lst.add_word("hi");
        lst.add_word("a");
        WHEN("Changing the root node from 'hi' to 'a") {
            lst.shift_root(lst.root->left);
            THEN("The root is 'a' and contains the information of the previous root") {
                vector<int> list_data{2, 2, 2};
                words expected{"a"};
                REQUIRE(list_data == lst.list_info());
                REQUIRE(expected == lst.words_in_order());
            }
        }
         
    }
}

TEST_CASE("left-rotation") {
    SUBCASE("Rotating an unbalanced tree") {
        GIVEN("An unbalanced tree with a larger left subtree") {
            WordlistTest lst;
            lst.add_word("d");
            lst.add_word("b");
            lst.add_word("a");
            WHEN("Balancing the list") {
                lst.left_rotation(lst.root);
                THEN("The tree is balanced") {
                    words expected{"a", "b", "d"};
                    REQUIRE(expected == lst.words_in_order());
                }
            }
        }
    }
}