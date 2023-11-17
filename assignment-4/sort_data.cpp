#include "a4_sort_implementations.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <functional>
using namespace std;


/**
 * @brief Generates the input sizes to test for
 * 
 * @return vector<int> all the input sizes for the collections to test
 */
vector<int> gen_input_sizes() {
  vector<int> input_values;
  for (int i = 0; i < 25; i++) {
    input_values.push_back((i + 1) * 2000);
  }
  return input_values;
}

ofstream &operator<<(ofstream &os, const Sort_stats &ss)
{
    os << ss.sort_name << ", "
       << ss.vector_size << ", "
       << ss.num_comparisons
       << ", " << ss.cpu_running_time_sec;
    return os;
}

typedef vector<vector<int>> InputData;

/**
 * @brief Takes a collection and returns eight copies of that collection
 * 
 * @param src the collection to copy
 * @return InputData the eight copies of the original collection
 */
InputData mk_copies(const vector<int>& src) {
    return {src, src, src, src, src, src, src, src};
}

vector<Sort_stats> get_sort_info(InputData& data) {
    Sort_stats quick_info = quick_sort(data[0]);
    Sort_stats insertion_info = insertion_sort(data[1]);
    Sort_stats priority_info = priority_queue_sort(data[2]);
    Sort_stats iquick_info = iquick_sort(data[3]);
    Sort_stats merge_info = merge_sort(data[4]);
    Sort_stats selection_info = selection_sort(data[5]);
    Sort_stats shell_info = shell_sort(data[6]);
    Sort_stats bubble_info = bubble_sort(data[7]);
    return {quick_info, insertion_info, priority_info, iquick_info, merge_info, selection_info, shell_info, bubble_info};
}

/**
 * @brief Takes an osftream and returns a function that prints the performance of all the sorting algorithms
 * given a certain collection to order
 * 
 * @param os the osftream
 * @return function<void(int)> a function that prints the performance of each algorithm
 */
function<void(int)> print_data_for_n_elems(ofstream& os) {
    return [&os](int input_size) {
      cout << "The size to test for " << input_size << endl;
      vector<int> src = rand_vec(input_size, -input_size / 2, input_size / 2);
      InputData data = mk_copies(src);
      vector<Sort_stats> all_sort_info = get_sort_info(data);
      for_each(all_sort_info.begin(), all_sort_info.end(),
               [&os](Sort_stats& info) { os << info << endl; });
    };
}


/**
 * @brief Prints the information of the performances of the sorting algorithms given N elements, 
 * where N ranges from 2000 to 50,000.
 * 
 */
void print_sort_data(const vector<int>& input_sizes, ofstream& os) {
    cout << "Sorting data " << endl;
    for_each(input_sizes.begin(), input_sizes.end(), print_data_for_n_elems(os));
}

int main() {
    ofstream os;
    vector<int> info = gen_input_sizes();
    cout << info << endl;
    os.open("sort_data_for_all_algorithms.txt");
      print_sort_data(info, os);
    os << "Writing this to a file." << endl;
    os << "another line.\n";
    os.close();
    return 0;
}