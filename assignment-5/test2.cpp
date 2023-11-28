#include "Wordlist.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <ostream>

template <typename T1, typename T2> 
ostream& operator<<(ostream& os, const pair<T1, T2>& heights) {
    os << "[" << heights.first << ", " << heights.second << "]";
    return os;
}

namespace doctest {

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

typedef vector<string> Words;

#define REQUIRE_WORD_STATS(list, different, total, fq, single)\
    REQUIRE(different == list.num_different_words());\
    REQUIRE(total == list.total_words());\
    REQUIRE(fq == list.most_frequent());\
    REQUIRE(single == list.num_singletons());

TEST_CASE("Wordlist from file") {
    SUBCASE("Reading from a large file") {
        GIVEN("A large text file") {
            WHEN("Adding all the content from the file") {
                WordlistTest lst = WordlistTest("tiny_shakespeare.txt");
                THEN("All the words from the file are in the list") {
                    REQUIRE(25670 == lst.num_different_words());
                    REQUIRE(202651 == lst.total_words());
                    REQUIRE(14919 == lst.num_singletons());
                    REQUIRE(5437 == lst.get_count("the"));
                    REQUIRE("the 5437" == lst.most_frequent());
                }
            }
        }
    }
}