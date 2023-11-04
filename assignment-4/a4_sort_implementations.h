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

int random_sign(int min) {
    return min >= 0 ? 1 : rand() % 2 == 0 ? 1 : -1;
}

/**
 * @brief Takes a minimum and a maximum number and generates a random number within those two numbers
 * 
 * @param min the smallest possible number
 * @param max the largest possible number
 * @return int a number that lies in between min and max
 */
int rand_num(int min, int max) {
    int end = max >= 0 ? max + 1 : min  - 1;
    int num = random_sign(min) * (rand() + min) % end;
    // Continue generating numbers until one is within
    // min and max
    while (min > num || num > max) {
        num = random_sign(min) * ((num + min) % end);
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
    // advance(iter, start_pos);
    for(int i = 0; i < start_pos; i++) {
        iter++;
    }
    return iter;
}

template <typename T> int min_elem_pos(const vector<T>& coll, int start_pos) {
    T min = coll[start_pos];
    int min_index = start_pos;
    auto start = coll.begin();
    for(auto curr = start_iter(coll, start_pos + 1); curr != coll.end(); curr++) {
        if (min > *curr) {
            min = *curr;
            min_index = distance(start, curr);
        }
    }
    return min_index;
}

template <typename T> void swap(vector<T>& coll, int fst, int snd) {
    T tmp = coll[fst];
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


/**
 * @brief Takes a collection, an end position, and moves the 
 * largest value in the collection to the end
 * 
 * @tparam T 
 * @param coll the collection to look through
 * @param end the last position to look at
 * @param info the information about the operations occuring with bubble sort
 */
template <typename T> void bubble_swap(vector<T> &coll, int end, Sort_stats& info) {
    for(int pos = 0; pos < end; pos++) {
        if (coll[pos] > coll[pos + 1]) {
            swap(coll, pos, pos + 1);
            info.num_comparisons++;
        }
    }
}

template <typename T> Sort_stats bubble_sort(vector<T> &coll) {
    Sort_stats info{"bubble sort", coll.size(), 0, 0};
    
    clock_t start = clock();

    // Move largest element to the end, and then do the same 
    // on the rest of the collection
    int final_pos = coll.size() - 1;
    while (final_pos > 0) {
        bubble_swap(coll, final_pos, info);
        final_pos--;
    }
    clock_t end = clock();
    info.cpu_running_time_sec = double(end - start) / CLOCKS_PER_SEC;

    return info;
}

/**
 * @brief Takes a collection and a start position and moves the element at the start position
 *  to its correct location
 * 
 * @tparam T 
 * @param coll the collection passed
 * @param start the position where the element to move will be
 * @param info the current information about the operations that occurred while sorting
 * @return Sort_stats information about the operations occuring while sorting
 */
template <typename T> Sort_stats insert_sort_order(vector<T>& coll, int end, Sort_stats& info) {
    int curr = end;
    // Continue swapping element until there 
    // are no more elements or the element is in its correct spot
    while (curr != 0 && coll[curr] < coll[curr - 1]) {
            swap(coll, curr, curr - 1);
            info.num_comparisons++;
            curr--;
        }
    return info;
}

template <typename T> Sort_stats insertion_sort(vector<T> &v) {
    Sort_stats info{"insertion sort", v.size(), 0, 0};
    clock_t start = clock();

    // Ordering the elements by moving the element
    // at the current position to its correct spot
    for(int pos = 1; pos < v.size(); pos++) {
        insert_sort_order(v, pos, info);
    }
    clock_t end = clock();
    info.cpu_running_time_sec = double(end - start) / CLOCKS_PER_SEC;

    return info;
}

template <typename T> void add_remaining_items(vector<T>& dest, vector<T>& src, int curr, int end) {
    if (curr > end) {
        return;
    }
    while (curr <= end) {
        dest.push_back(src[curr]);
        curr++;
    }
}

struct NextElems {
    int fst;
    int snd;
    NextElems(int first, int second): fst(first), snd(second) {};
};

bool operator==(const NextElems& a, const NextElems& b) {
    return a.fst == b.fst && a.snd == b.snd;
}

/**
 * @brief Takes a sorted collection, another collection and two locations in the the secondary collection 
 * and adds the smallest of the values in both locations to the sorted collection. Returns the 
 * locations of the new numbers to try
 * 
 * @param sorted the sorted collection
 * @param cpy_from the collection to copy from
 * @param fst the first position in cpy_from to look at
 * @param snd the second position in cpy_from to look at
 * @return NextElems the next positions in cpy_from to compare
 */
template <typename T> NextElems add_smallest_elem(vector<T>& sorted, vector<T>& cpy_from, int fst, int snd) {
    T fst_val = cpy_from[fst];
    T snd_val = cpy_from[snd];

    if (fst_val < snd_val) {
        sorted.push_back(fst_val);
        fst++;
    } else {
        sorted.push_back(snd_val);
        snd++;
    }

    return NextElems{fst, snd};
}

/**
 * @brief Takes a collection to overwrite, a collection to copy from, and a starting and ending point 
 * for the collection to copy from. Copies all the elements from the start to the end point
 * in the second collection to the first collection
 * 
 * @tparam T 
 * @param src the collection to copy to
 * @param cpy_from the collection to copy from
 * @param start the point from where to stop copying
 * @param end the last position to copy from
 * @return vector<T> the overwritten collection
 */
template <typename T> vector<T>& overwrite_coll(vector<T>& src, vector<T>& cpy_from, int start) {
    auto curr = src.begin();
    advance(curr, start);
    auto cpy_val = cpy_from.begin();
    while (cpy_val != cpy_from.end()) {
        *curr = *cpy_val;
        cpy_val++;
        curr++;
    }
    return src;
}

/**
 * @brief takes a collection, a start and end point, and a middle point inbetween them, and sorts the 
 * items ascending in the collection from the start to end point
 * 
 * @tparam T 
 * @param coll the collection passed
 * @param start the point from where to start sorting
 * @param mid the middle point in between start and end
 * @param end the point in the collection to stop sorting at
 */
template <typename T> vector<T>& merge(vector<T>& coll, int start, int mid, int end) {
    vector<T> sorted_portion;
    int curr_first = start, curr_snd = mid;
    while (curr_first < mid && curr_snd <= end) {

        NextElems next = add_smallest_elem(sorted_portion, coll, curr_first, curr_snd);
        if (curr_first != next.fst) {
            curr_first = next.fst;
        } else {
            curr_snd = next.snd;
        }
    }

    add_remaining_items(sorted_portion, coll, curr_first, mid - 1);
    add_remaining_items(sorted_portion, coll, curr_snd, end);
    return overwrite_coll(coll, sorted_portion, start);
}

template <typename T> void merge_sort_order(vector<T>& coll, int start, int mid, int final) {
    if (start == final || start > final) {
        return;
    }

    int lower_end = mid - 1;
    int lower_mid = (lower_end - start + 1) / 2;
    int actual_low_mid = lower_mid + start;
    merge_sort_order(coll, start, actual_low_mid, lower_end);

    int high_mid = (final - mid + 1) / 2;
    int actual_high_mid = mid + high_mid;
    merge_sort_order(coll, mid, actual_high_mid, final);
    merge(coll, start, mid, final);
}

template <typename T> Sort_stats merge_sort(vector<T> &v) {
    Sort_stats info{"merge sort", v.size(), 0, 0};
    clock_t start = clock();

    // Ordering the elements by moving the element
    // at the current position to its correct spot
    int final_pos = v.size() - 1;
    int mid = (final_pos + 1) / 2;
    merge_sort_order(v, 0, mid, final_pos);
    clock_t end = clock();
    info.cpu_running_time_sec = double(end - start) / CLOCKS_PER_SEC;

    return info;
}

struct SwapLocations {
    int small;
    int large;
    SwapLocations(int a, int b): small(a), large(b){};
};

bool operator== (const SwapLocations& a, const SwapLocations& b) {
    return a.large == b.large && a.small == b.small;
}


template <typename T> int larger_than(const vector<T>& coll, int start, int end,const T& pivot) {
    int curr_pos = start;
    while (curr_pos <= end && coll[curr_pos] <= pivot) {
        curr_pos++;
    }
    return curr_pos > end ? -1 : curr_pos;
}

template <typename T> int smaller_than(const vector<T>& coll, int start, int end,const T& pivot) {
    int curr_pos = start;
    while (curr_pos >= end && coll[curr_pos] >= pivot) {
        curr_pos--;
    }
    return curr_pos < end ? -1 : curr_pos;
}

/**
 * @brief Takes a collection, a starting and ending location, and a pivot and returns the position 
 * of an element larger or smaller than the pivot depending on the start and end location
 * 
 * @tparam T 
 * @param coll the collection to search over
 * @param start the location in the collection to start at
 * @param end the location in the collection to stop searching at
 * @param pivot the element which divides the collection
 * @return int the position of the element larger or smaller than the pivot if it exists. returns
 * -1 otherwise.
 */
template <typename T> int find_elem(const vector<T>& coll, int start, int end,const T& pivot) {
    if (start < end) {
        return larger_than(coll, start, end, pivot);
    }

    return smaller_than(coll, start, end, pivot);
}

/**
 * @brief Takes a collection, the first and final indexes of the collection, the value of the pivot,
 *  and returns the indexes of the next items to swap.
 * 
 * @tparam T 
 */
template <typename T> SwapLocations find_swap_pair(vector<T>& coll, int start, int end) {
    T pivot_val = coll[(end - start + 1) / 2];
    return SwapLocations(find_elem(coll, end, start, pivot_val), find_elem(coll, start, end, pivot_val));
}


bool overlapping(const SwapLocations& locs) {
    return locs.large > locs.small || locs.small < locs.large;
}

bool outside_range(const SwapLocations& locs) {
    return locs.large == -1 || locs.small == -1;
}

bool has_valid_locations(const SwapLocations& locs) {
    return !outside_range(locs) && !overlapping(locs); 
}

// template <typename T>
// void quick_order(vector<T>& coll, int start, int end) {
//     if (start == end) {
//         return;
//     }

//     int pivot_loc = (end - start + 1) / 2;
//     T pivot_val = coll[pivot_loc];
//     SwapLocations swap = find_swap_pair(coll, start, end, pivot_val);
//     while (has_valid_locations(swap)) {
//         swap(coll, swap.large, swap.small);
//         swap = find_swap_pair(coll, swap.large, swap.small, pivot_val);
//     }

//     if (no_elem_>_than_pivot(swap.large, end)) {
//         swap(end, pivot_loc);
//         quick_order(coll, start, end - 1);
//     }
//     elseif(no_elem_<_than_pivot(swap.small, start)) {
//         swap(start, pivot_loc);

//     } else {
//         quick_order(coll, start, pivot_loc - 1);
//         quick_order(coll, pivot_loc + 1, end);
//     }
// }

// template <typename T> Sort_stats quick_sort(vector<T> &v) {
// // 1: crear el Sort_stats objeto
// // 2: llamar quicksort con el pivot.
// // 3: devolver el objeto
//     Sort_stats info{"quick sort", v.size(), 0, 0};
//     clock_t start = clock();

//     quick_order(v, 0, v.size() - 1);
//     clock_t end = clock();
//     info.cpu_running_time_sec = double(end - start) / CLOCKS_PER_SEC;

//     return info;
// }

//
// Put the implementations of all the functions listed in a4_base.h here, as
// well as is_sorted and rand_vec. You can use other helper functions if needed.
//
