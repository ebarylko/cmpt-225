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
 * @brief Takes a collection and returns true if it sorted in ascending order.
 * False otherwise
 * @tparam T the type of the items in the collections
 * @param coll the collection of items
 * @return true if the collection is sorted in ascending order
 * @return false if the above is not true
 */
template <typename T> bool is_sorted(vector<T>& coll) {
    if (coll.size() <= 1) {
        return true;
    }
    auto curr = coll.cbegin();
    auto end = coll.cend() - 1;
    // // See if list is ordered and obtain the first element where 
    // // order does not hold (if unordered)
    while(curr != end && *curr <= *(curr + 1)) {
        curr++;
    };
    return curr == end;
}

int rand_num(int min, int max) {
    int end = max + 1;
    int num = (rand() + min) % end;
    while (min > num || num > max) {
        num = (num + min) % end;
    }
    return num;
}

/**
 * @brief Takes a collection of size N, a max and minimum number, and generates 
 * a collection of size N of numbers ranging from the minimum to the maximum 
 * 
 * @param size the size of the collection
 * @param min the smallest possible number
 * @param max the largest possibe number
 * @return vector<int> a collection of numbers ranging from min to max inclusive
 */
vector<int> rand_vec(int size, int min, int max) {
    vector<int> rand_nums;
    // Seeding the random number generator
    srand(time(0));
    // Adjusting the numbers so they are within 
    // min and max
    for(int i = 0; i < size; i++) {
        rand_nums.push_back(rand_num(min, max));
    }
    return rand_nums;
}

//
// Put the implementations of all the functions listed in a4_base.h here, as
// well as is_sorted and rand_vec. You can use other helper functions if needed.
//
