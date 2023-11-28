#define TESTING 1
#include "Wordlist.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <ostream>

typedef WordlistTest::Heights Heights;
typedef WordlistTest::SubtreeHeights node_heights;
template <typename T1, typename T2> 
ostream& operator<<(ostream& os, const pair<T1, T2>& heights) {
    os << "[" << heights.first << ", " << heights.second << "]";
    return os;
}


typedef WordlistTest::ListData ListData;

namespace doctest
{

template <typename T> struct StringMaker<vector<T>> {
     static String convert(const vector<T>& in) {
        stringstream os;
        os << "[";
        for (auto it = in.cbegin(); it != in.cend();) {
            os << *it;
            if (++it != in.cend()) { os << ", "; }
        }
        os << "]";
        return os.str().c_str();
}
};


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
struct StringMaker<ListData>
{
    static String convert(const ListData& in) {
        ostringstream oss;

        oss << "[";
        oss << "(" << get<0>(in) << ", " << get<1>(in) << ", " << get<2>(in) << ", " << get<3>(in) << ", " << get<4>(in)  << ")";

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

template <>
struct StringMaker<vector<pair<string, int>>>
{
    static String convert(const vector<pair<string, int>>& in) {
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
}

/**
 * @brief Takes a collection of expected heights and creates a collection of 
 * heights of the nodes visited using inorder traversal
 * 
 * @param all_heights the expected heights
 * @return heights the collection of heights corresponding to the nodes
 * in alphabetical order
 */
Heights mk_heights(initializer_list<pair<int,int>> all_heights) {
    Heights tree_heights{all_heights};
    return tree_heights;
}

typedef vector<string> Words;

#define REQUIRE_WORD_STATS(list, different, total, fq, single)\
    REQUIRE(different == list.num_different_words());\
    REQUIRE(total == list.total_words());\
    REQUIRE(fq == list.most_frequent());\
    REQUIRE(single == list.num_singletons());

TEST_CASE("Wordlist from file") {
    // SUBCASE("Reading from a large file") {
    //     GIVEN("A large text file") {
    //         WHEN("Adding all the content from the file") {
    //             WordlistTest lst = WordlistTest("tiny_shakespeare.txt");
    //             THEN("All the words from the file are in the list") {
    //                 REQUIRE(25670 == lst.num_different_words());
    //                 REQUIRE(202651 == lst.total_words());
    //                 REQUIRE("the 5437" == lst.most_frequent());
    //                 REQUIRE(14919 == lst.num_singletons());
    //             }
    //         }
    //     }
    // }
    SUBCASE("Just a few words") {
        GIVEN("A few words") {
            string input("A few words");
            WHEN("Loading the words") {
                stringstream stream(input);
                WordlistTest lst;
                lst.load_from(stream);
                THEN("The stats match the few words") {
                    REQUIRE_WORD_STATS(lst, 3, 3, "A 1", 3);
                }
            }
        }
    }

}
