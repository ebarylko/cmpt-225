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
#include <iterator>
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

/**
 * @brief Takes a minimum and a maximum number and generates a random number within those two numbers
 * 
 * @param min the smallest possible number
 * @param max the largest possible number
 * @return int a number that lies in between min and max
 */
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

template <typename T> auto start_iter(const vector<T>& coll, int start_pos) {
    auto iter = coll.begin();
    advance(iter, start_pos);
    return iter;
}

template <typename T> int min_elem_pos(const vector<T>& coll, int start_pos) {
    T min = coll[start_pos];
    int min_index = start_pos;
    auto start = coll.begin();
    for(auto curr = start_iter(coll, start_pos); curr != coll.end(); curr++) {
        if (min > *curr) {
            min = *curr;
            min_index = distance(start, curr);
        }
    }
    return min_index;
}

template <typename T> void swap(vector<T> coll, int fst, int snd) {
    T& tmp = coll[fst];
    coll[fst] = coll[snd];
    coll[snd] = tmp;
}

template <typename T>
Sort_stats selection_sort(vector<T> &v) {
    Sort_stats info{"selection sort", v.size(), 0, 0};

    clock_t start = clock();
    // finding the smallest element and moving it to the front in each iteration
    for(int pos = 0; pos < v.size(); pos++) {
        info.num_comparisons += v.size() - 1 - pos;
        int min_index = min_elem_pos(v, pos);
        swap(v, pos, min_index);
    }

    clock_t end = clock();
    info.cpu_running_time_sec = double(end - start) / CLOCKS_PER_SEC;
    return info;
}

//
// Put the implementations of all the functions listed in a4_base.h here, as
// well as is_sorted and rand_vec. You can use other helper functions if needed.
//
