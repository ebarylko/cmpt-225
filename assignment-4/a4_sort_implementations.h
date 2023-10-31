// a4_sort_implementations.h

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : Eitan Barylko
// St.# : 301559349
// Email: eab16@sfu.ca
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

#pragma once

#include "a4_base.h"
//
// Do NOT add any other #includes to this file!
//

using namespace std;

/**
 * @brief Takes a current position and an end position and returns true 
 * if the current position is the end position. False otherwise.
 * 
 * @param pos the current position
 * @param end the final position
 * @return true if pos != end
 * @return false if the above is not true
 */
bool not_at_end(int pos, int end) {
    return pos != end;
}

/**
 * @brief Takes two elements and returns true if the first element is less than or equal
 * than the second
 * 
 * @tparam T the type of the elements
 * @param a the first element
 * @param b the second element
 * @return true if a <= b
 * @return false if the above is not true
 */
template <typename T> bool is_ordered(T& a, T& b) {
    return a <= b;
}

/**
 * @brief Takes a collection and returns true if it sorted in ascending order.
 * False otherwise
 * @tparam T the type of the items in the collections
 * @param coll the collection of items
 * @return true if the collection is sorted in ascending order
 * @return false if the above is not true
 */
template <typename T> bool is_sorted(vector<T>& coll) {
    int pos = 0;
    int final_pos = coll.size() - 1;
    // See if list is ordered and obtain the first element where 
    // order does not hold (if unordered)
    while(not_at_end(pos, final_pos) && is_ordered(coll[pos], coll[pos + 1])) {
        pos++;
    }
    return !not_at_end(pos);
}


//
// Put the implementations of all the functions listed in a4_base.h here, as
// well as is_sorted and rand_vec. You can use other helper functions if needed.
//
