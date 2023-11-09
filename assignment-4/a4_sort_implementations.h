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

/**
 * @brief Takes a number and generates 1 or -1 if the number is negative. Generates 1 otherwise.
 * 
 * @param num the number given
 * @return int the sign to be used
 */
int random_sign(int num) {
    return num >= 0 ? 1 : rand() % 2 == 0 ? 1 : -1;
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
    int num = random_sign(min) * ((rand() % end) + min);
    // Continue generating numbers until one is within
    // min and max
    while (min > num || num > max) {
        num = random_sign(min) * ((num + min) % end);
    }
    return num;
}

/**
 * @brief Takes a collection of size N, a maximum and minimum number, and generates 
 * a collection of size N of numbers in the range [minimum, maximum]
 * 
 * @param size the size of the collection
 * @param min the smallest possible number
 * @param max the largest possibe number
 * @return vector<int> a collection of numbers ranging from min to max inclusive
 */
vector<int> rand_vec(int size, int min, int max) {
    vector<int> rand_nums;
    /**
    Seeding the random number generator
     */
    srand(time(0));
    /**
     * Adding the random numbers
     */
    for(int i = 0; i < size; i++) {
        rand_nums.push_back(rand_num(min, max));
    }
    return rand_nums;
}

/**
 * @brief Takes a collection and a starting position 
 * and returns an iterator to the element at the starting position
 * 
 * @tparam T 
 * @param coll the collection given
 * @param start_pos the position to start at
 * @return auto an iterator to the element at start_pos
 */
template <typename T> auto shift_iter(const vector<T>& coll, int start_pos) {
    auto iter = coll.begin();
    // Shifting iterator to the correct position
    for(int i = 0; i < start_pos; i++) {
        iter++;
    }
    return iter;
}

/**
 * @brief Take a collection and a start position S and returns the position of the smallest
 * element in the collection starting from S 
 * 
 * @tparam T 
 * @param coll the collection to search through
 * @param start_pos the position to start at
 * @return int the index of the smallest element in the collection starting from S
 */
template <typename T> int min_elem_pos(const vector<T>& coll, int start_pos) {
    T min = coll[start_pos];
    int min_index = start_pos;
    auto start = coll.begin();

    // Updating the current smallest element if a smaller one is found
    for(auto curr = shift_iter(coll, start_pos + 1); curr != coll.end(); curr++) {
        if (min > *curr) {
            min = *curr;
            min_index = curr - start;
        }
    }
    return min_index;
}

/**
 * @brief Takes a collection and two positions in the collectioon, and swaps the elements
 * at those positions
 * 
 * @tparam T 
 * @param coll the collection given
 * @param fst the position of the first element
 * @param snd the position of the second element
 */
template <typename T> void swap(vector<T>& coll, int fst, int snd) {
    T tmp = coll[fst];
    coll[fst] = coll[snd];
    coll[snd] = tmp;
}

template <typename T>
Sort_stats selection_sort(vector<T> &v) {
    Sort_stats info{"selection sort", v.size(), 0, 0};

    clock_t start = clock();
    // finding the smallest element and moving it to the front each time
    for(int pos = 0; pos < v.size(); pos++) {

        /**
         * @brief 
        I always set the starting element as my smallest value 
        and compare the other values only making N - 1 comparisons. 
        Adjusting for a new starting point, I also subtract my 
        current position to get the true amount of comparisons.
         */
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
 * @param info the amount of comparisons done so far
 */
template <typename T> void bubble_swap(vector<T> &coll, int end, Sort_stats& info) {
    // Moving the largest element in the range from [0, end) all
    // the way to the end
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
    // for the rest of the collection
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
template <typename T> Sort_stats order_elem_in_coll(vector<T>& coll, int start, int end, Sort_stats& info) {
    int curr = end;
    // Continue swapping element until there 
    // are no more elements or the element is in its correct spot
    while (curr != start && coll[curr] < coll[curr - 1]) {
            swap(coll, curr, curr - 1);
            info.num_comparisons++;
            curr--;
        }
    return info;
}

/**
 * @brief Takes a collection, an initial and final position, and information about the
 * number of comparisons so far, and orders the collection in ascending order.
 * 
 * @tparam T 
 * @param coll the collection to order
 * @param start the first position in the collection
 * @param end the final position in the collection
 * @param info the amount of comparisons done so far
 */
template <typename T> void insert_sort_impl(vector<T>& coll, int start, int end, Sort_stats& info) {
    // Applying insertion sort on the collection, incrementing the ending
    // position each time
    for (int curr_end = start + 1; curr_end <= end; curr_end++) {
        order_elem_in_coll(coll, start, curr_end, info);
    }
}



template <typename T> Sort_stats insertion_sort(vector<T> &v) {
    Sort_stats info{"insertion sort", v.size(), 0, 0};
    clock_t start = clock();

    // Ordering the elements by moving the element
    // at the current position to its correct spot
    insert_sort_impl(v, 0, v.size() - 1, info);
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
template <typename T> vector<T>& merge(vector<T>& coll, int start, int mid, int end, Sort_stats& info) {
    vector<T> sorted_portion;
    int curr_first = start, curr_snd = mid;
    while (curr_first < mid && curr_snd <= end) {


        // Find the next smallest element to add and adjust the next elements to compare
        NextElems next = add_smallest_elem(sorted_portion, coll, curr_first, curr_snd);
        if (curr_first != next.fst) {
            curr_first = next.fst;
        } else {
            curr_snd = next.snd;
        }

        info.num_comparisons += 2;
    }

    add_remaining_items(sorted_portion, coll, curr_first, mid - 1);
    add_remaining_items(sorted_portion, coll, curr_snd, end);
    return overwrite_coll(coll, sorted_portion, start);
}

/**
 * @brief Takes a collection, a starting, middle, and end position, and information about 
 * the current amount of comparisons. Sorts the collection in ascending order
 * 
 * @tparam T 
 * @param coll the collection to order
 * @param start the position in the collection to start at
 * @param mid the position in the collection to end at
 * @param info the amount of comparisons done so far
 */
template <typename T> void merge_sort_order(vector<T>& coll, int start, int mid, int final, Sort_stats& info) {
    
   /**
    Do nothing if given invalid positions or a sorted subcollection
    */
    if (start == final || start > final) {
        return;
    }

    /**
     Ordering the subcollection from [start, mid)
     */
    int lower_end = mid - 1;
    int lower_mid = (lower_end - start + 1) / 2;
    int actual_low_mid = lower_mid + start;
    merge_sort_order(coll, start, actual_low_mid, lower_end, info);

    /**
     Ordering the subcollection from [mid, end]
     */
    int high_mid = (final - mid + 1) / 2;
    int actual_high_mid = mid + high_mid;
    merge_sort_order(coll, mid, actual_high_mid, final, info);

    /**
     Ordering the subcollections above
     */
    merge(coll, start, mid, final, info);
}

template <typename T> Sort_stats merge_sort(vector<T> &v) {
    Sort_stats info{"merge sort", v.size(), 0, 0};
    clock_t start = clock();

    int final_pos = v.size() - 1;
    int mid = (final_pos + 1) / 2;
    merge_sort_order(v, 0, mid, final_pos, info);
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


template <typename T> int find_larger_than(const vector<T>& coll, int start, int pivot_pos, Sort_stats& info) {
    T pivot = coll[pivot_pos];
    int curr_pos = start;
    while (curr_pos <= pivot_pos && coll[curr_pos] <= pivot) {
        curr_pos++;
        info.num_comparisons++;
    }
    return curr_pos > pivot_pos ? -1 : curr_pos;
}

template <typename T> int find_smaller_than(const vector<T>& coll, int start,int pivot_pos, Sort_stats& info) {
    int curr_pos = start;
    T pivot = coll[pivot_pos];
    while (curr_pos >= pivot_pos && coll[curr_pos] >= pivot) {
        curr_pos--;
        info.num_comparisons++;
    }
    return curr_pos < pivot_pos ? -1 : curr_pos;
}

/**
 * @brief Takes a collection, the first and final indexes of the collection, the value of the pivot,
 *  and returns the indexes of the next items to swap.
 * 
 * @tparam T 
 */
template <typename T> SwapLocations find_swap_pair(vector<T>& coll, int start, int end, int pivot_pos, Sort_stats& info) {
    return SwapLocations(find_smaller_than(coll, end, pivot_pos, info), find_larger_than(coll, start, pivot_pos, info));
}


bool overlapping(const SwapLocations& locs) {
    return locs.large > locs.small;
}

/**
 * @brief Takes a pair of locations L1, L2, and returns true if L1 and L2 are
 * sorted ascendingly. False otherwise.
 * 
 * @param locs the locations to compare
 * @return true if L1 and L2 is in ascending order
 * @return false if the above is untrue
 */
bool both_sides_sorted(const SwapLocations& locs) {
    return locs.large == -1 && locs.small == -1;
}

/**
 * @brief Takes two locations and returns true if the elements at those locations
 * can be swapped. False otherwise.
 * 
 * @param locs two locations to compare
 * @return true if the elements at the positions can be swapped
 * @return false if the above is not true
 */
bool can_swap(const SwapLocations& locs) {
    return !both_sides_sorted(locs); 
}

bool none_greater_than_pivot(int index) {
    return index == -1;
}

bool none_less_than_pivot(int index) {
    return index == -1;
}


/**
 * @brief Takes a pair of locations and returns true if the elements at those locations can be
 * swapped. False otherwise.
 * 
 * @param swap_info the pair of locations
 * @return true if the elems at the positions can be swapped
 * @return false if the above is untrue
 */
bool can_swap_both(const SwapLocations& swap_info) {
    return swap_info.large != -1 && swap_info.small != -1;
}

/**
 * @brief Takes a pair of locations with a smaller and larger element 
 * and returns true if only the smaller element can be swapped. False otherwise
 * 
 * @param info the pair of locations
 * @return true if only the smaller element can be swapped
 * @return false if the above is untrue
 */
bool can_swap_smaller(const SwapLocations& info) {
    return info.small != -1 && info.large == -1;
}

template <typename T> Sort_stats& nothing(vector<T>& coll, int pos, Sort_stats& info) {
    auto start = coll.begin();
    info.num_comparisons += pos - pos;
    return info;
}

#define INSERTION 0


/**
 * @brief Takes a collection, a starting and ending position, the amount of swaps done so far, and 
 * an option to apply another sorting algorithm and sorts the collection in ascending order.
 * 
 * @tparam T 
 * @param coll the collection given
 * @param start the starting point of the collection
 * @param end the ending point of the collection
 * @param info the number of swaps done so far
 * @param func_const an optional function to apply in certain scenarios
 */
template <typename T>
void quick_order(vector<T>& coll, int start, int end, Sort_stats& info, int func_const = 1) {
    /**
      Do nothing if given invalid positions
     */
    if (start >= end) {
        return;
    }

    /**
     Apply optional functions if certain conditions 
     are met
     */
    if (end - start <= 12 && func_const == INSERTION) {
        insert_sort_impl(coll, start, end, info);
        return;
    }

    int pivot_loc = start + (end - start + 1) / 2;
    SwapLocations swap_info = find_swap_pair(coll, start, end, pivot_loc, info);
    

    /**
      Continue swapping elements until all elements greater than the pivot are
      to the right of it and all elements smaller than the pivot are to the left of it
     */
    while (can_swap(swap_info)) {
        if (can_swap_both(swap_info)) {
            swap(coll, swap_info.large, swap_info.small);
        }
         /**
          If only smaller element can be swapped,
          interchange it with pivot and adjust pivot and smaller element
          location
          */
         else if (can_swap_smaller(swap_info)) {
            swap(coll, swap_info.small, pivot_loc);
            swap_info.large = pivot_loc;
            pivot_loc = swap_info.small;
        } 
         /**
          If only larger element can be swapped,
          interchange it with pivot and adjust pivot and larger element
          location
          */
        else {
            swap(coll, swap_info.large, pivot_loc);
            swap_info.small = pivot_loc;
            pivot_loc = swap_info.large;
        }

        swap_info = find_swap_pair(coll, swap_info.large, swap_info.small, pivot_loc, info);
    }
    /**
     Order the subcollections from [start, pivot) and from
    [pivot, end]
     */
    quick_order(coll, start, pivot_loc - 1, info, func_const);
    quick_order(coll, pivot_loc + 1, end, info, func_const);
}

template <typename T> Sort_stats quick_sort(vector<T> &v) {
    Sort_stats info{"quick sort", v.size(), 0, 0};
    clock_t start = clock();

    quick_order(v, 0, v.size() - 1, info);
    clock_t end = clock();
    info.cpu_running_time_sec = double(end - start) / CLOCKS_PER_SEC;

    return info;
}

/**
 * @brief Takes the current position and the final position
 * of the collection and returns true if we are not at the end of the collection
 * 
 * @param curr_pos the current position
 * @param end the final index of the collection
 * @return true if we are not at the end of the collection
 * @return false if the above is not true
 */
bool is_pair_within_coll(int curr_pos, int end) {
    return curr_pos < end;
}

/**
 * @brief Takes a collection, a distance G, and the current number of comparisons and 
 * orders every pair of elements seperated by G ascendingly
 * 
 * @tparam T 
 * @param coll the collection being ordered
 * @param gap the distance between the elements being compared
 * @param info the amount of comparisons done so far 
 */
template <typename T> void order_elems_by_gap(vector<T>& coll, int gap, Sort_stats& info) {
    int curr_pos = 0;
    int gap_elem = curr_pos + gap;
    // hacer que el segun elemento esta con un nombre
    while (is_pair_within_coll(gap_elem, coll.size())) {
        if (coll[curr_pos] > coll[gap_elem]) {
            info.num_comparisons++;
            swap(coll, curr_pos, gap_elem);
        }
        curr_pos++;
        gap_elem++;
    }
}

/**
 * @brief Takes a collection and the current number of comparisons and returns
 * the collection in ascending order
 * 
 * @tparam T 
 * @param coll the collection to sort
 * @param info the amount of comparisons done so far
 */
template <typename T> void shell_sort_impl(vector<T>& coll, Sort_stats& info) {
    int gap = coll.size() / 2;

    /**
    Leaving since collection is already sorted
     */
    if (gap == 0) {
        return;
    }

    /**
    Order the pairs of elements spaced by the gap and
    reduce the gap for the next iteration
     */
    while (gap != 1) {
        order_elems_by_gap(coll, gap, info);
        gap /= 2;
    }

    /**
      calling insertion sort on the final pass
     */
    insert_sort_impl(coll, 0, coll.size() - 1, info);
}

template <typename T>
Sort_stats shell_sort(vector<T> &v) {
    Sort_stats info{"shell sort", v.size(), 0, 0};
    clock_t start = clock();

    shell_sort_impl(v, info);
    clock_t end = clock();
    info.cpu_running_time_sec = double(end - start) / CLOCKS_PER_SEC;

    return info;
}


template <typename T> Sort_stats iquick_sort(vector<T> &v) {
    Sort_stats info{"iquick sort", v.size(), 0, 0};
    clock_t start = clock();

    quick_order(v, 0, v.size() - 1, info, 0);
    clock_t end = clock();
    info.cpu_running_time_sec = double(end - start) / CLOCKS_PER_SEC;

    return info;
}

/**
 * @brief Takes a position in the heap and returns the location of the parent for that position
 * 
 * @param child a position in the heap
 * @return int the position of the parent of child
 */
int parent(int child) {
    return child == 0 ? -1 : (child - 1) / 2;
}

/**
 * @brief Takes a collection and the position of a child and its parent, and returns true if the
 * parent is within the collection and is greater than its child. False otherwise
 * 
 * @tparam T 
 * @param coll the collection given
 * @param child_pos the location of the child in the collection
 * @param parent_pos the location of the parent in the collection
 * @return true if the parent has a valid location and is greater than its child
 * @return false if the condition above goes unsatisfied
 */
template <typename T> bool smaller_than_parent(vector<T>& coll, int child_pos, int parent_pos, Sort_stats& info) {
    if (parent_pos == -1) {
        return 0;
    }
    if (coll[child_pos] >= coll[parent_pos]) {
        info.num_comparisons++;
        return 0;
    }
    return 1;
}

template <typename T>
bool bigger_than_child(const vector<T>& coll, int parent_pos, int child_pos, Sort_stats& info) {
    if (child_pos < coll.size() && coll[parent_pos] > coll[child_pos]) {
        info.num_comparisons++;
        return 1;
    }
    return 0;
}

template <typename T>
bool is_bigger_than_children(vector<T>& coll, int parent_pos, Sort_stats& info) {
    return bigger_than_child(coll, parent_pos, 2 * parent_pos + 1, info) ||
           bigger_than_child(coll, parent_pos, 2 * parent_pos + 2, info);
}


template <typename T> bool not_in_range(const vector<T>& coll, int child_pos) {
    int size = coll.size();
    return child_pos >= size;
}

template <typename T> int find_smallest_child(const vector<T>& coll, int parent_pos, Sort_stats& info) {
    int child_1 = 2 * parent_pos + 1;
    int child_2 = 2 * parent_pos + 2;
    if (not_in_range(coll, child_2)) {
        return child_1;
    }

    info.num_comparisons++;
    return coll[child_1] > coll[child_2] ? child_2 : child_1;
}

template <typename T>
class Heap {
    vector<T> coll;
    Sort_stats info;
    /**
     * @brief Takes the position of the element to move and adjusts it so it is at a location
     * where it satisfies the heap property
     * 
     * @param elem_to_move the index of the element to move up
     */
    void bubble_up(int elem_to_move) {
        int parent_pos = parent(elem_to_move);

        /**
         * @brief Swapping the element upwards until it is in the correct position
         * 
         */
        while (smaller_than_parent(this->coll, elem_to_move, parent_pos, this->info)) {
            swap(this->coll, elem_to_move, parent_pos);
            elem_to_move = parent_pos;
            parent_pos = parent(elem_to_move);
        }
    }

    void bubble_down(int elem_pos) {
        if (this->is_empty()) {
            return;
        }

        while (is_bigger_than_children(coll, elem_pos, this->info)) {
            int child_to_swap = find_smallest_child(coll, elem_pos, this->info);
            swap(coll, elem_pos, child_to_swap); 
            elem_pos = child_to_swap;
        }
    }

    public: 
    ~Heap() {};
    
    int size() const {
        return this->coll.size();
    }

    bool is_empty() const {
        return !this->size();
    }

    void insert(const T& item) {
        coll.push_back(item);
        bubble_up(coll.size() - 1);
    }

    void insert_n(const vector<T>& items) {
        for (T item : items) {
            this->insert(item);
        }
    }

    vector<T> items() {
        return vector<T>(this->coll);
    }

    void remove_min() {
        if (!this->is_empty()) {
            swap(this->coll, 0, coll.size() - 1);
            this->coll.pop_back();
            bubble_down(0);
        }
    }

    T min() {
        return this->coll[0];
    }

    Sort_stats& sort_info() {
        this->info.sort_name = "priority queue sort";
        return this->info;
    }
};


template <typename T> class Priority_Queue {
    Heap<T> t;
    public:
    void insert(T& item) {
        t.insert(item);
    }
    void remove_min() {
        t.remove_min();
    }
    const T min() {
        return t.min();
    }
    int size() {
        return t.size();
    }

    void insert_n(vector<T>& coll) {
        t.insert_n(coll);
    }

    Sort_stats& sort_info() {
        return this->t.sort_info();
    }

    void sort(vector<T>& coll) {
        this->t.insert_n(coll);
        coll.clear();
        while (!this->t.is_empty()) {
            coll.push_back(this->min());
            this->remove_min();
        }
    }
};


template <typename T>
Sort_stats priority_queue_sort(vector<T> &v) {
    Priority_Queue<T> pq;
    Sort_stats& info = pq.sort_info();
    info.vector_size = v.size();

    clock_t start = clock();

    pq.sort(v);

    clock_t end = clock();
    info.cpu_running_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return info;
}

//
// Put the implementations of all the functions listed in a4_base.h here, as
// well as is_sorted and rand_vec. You can use other helper functions if needed.
//
