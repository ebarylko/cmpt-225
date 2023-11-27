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


TEST_CASE("data") {
    SUBCASE("A small list") {
        GIVEN("A small list of words") {
            // WordlistTest lst = WordlistTest("small.txt");
            WordlistTest lst;
            string word;
            ifstream file("small.txt");
            // Add the words to the list while there is input to process

            while (file >> word)
            {
                cout << "-- Word: " << "$" <<  word  << "$" << endl;
                lst.add_word(word);
                // lst.add_word("is");
                // lst.add_word("a");


                auto words = lst.words_in_order();
                for_each(words.begin(), words.end(), [](string word) {cout << word << ", ";});
                cout << endl;
            }

            file.close();
            WHEN("Getting the information about the list") {
                ListData actual = lst.data();
                THEN("It contains the data about the list") {
                    vector<string> actual_words = lst.words_in_order();
                    vector<string> expected_words{"This", "a", "is", "or", "test", "test?", "this"};
                    REQUIRE(expected_words == actual_words);
                    REQUIRE(9 == lst.total_words());

                    ListData expected{7, 9, "a 2", 5, 71};
                    REQUIRE(expected == actual);
                }
                
            }
        }
    }
}