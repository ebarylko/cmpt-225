#include "a4_sort_implementations.h"
#include <algorithm>
#include <fstream>
#include <functional>
using namespace std;


vector<int> gen_input_sizes() {
  vector<int> input_values;
  for (int i = 0; i < 25; i++) {
    input_values[i] = (i + 1) * 2000;
  }
  return input_values;
}

ofstream &operator<<(ofstream &os, const Sort_stats &ss)
{
    os << "Sort_stats{" << ss.sort_name
       << ", size=" << ss.vector_size
       << ", num_comparisons=" << ss.num_comparisons
       << ", cpu_running_time_sec=" << ss.cpu_running_time_sec
       << "}";
    return os;
}

function<void(int)> print_data_for_n_elems(ofstream& os) {
    return [&os](int input_size) {
    vector<int> src = rand_vec(input_size, -input_size / 2, input_size / 2);
    InputData data = gen_copies(src);
    vector<Sort_stats> all_sort_info = get_sort_info(data);
    for_each(all_sort_info.begin(), all_sort_info.end(), [&os](Sort_stats& info) {os << info << endl;})
    };
}


/**
 * @brief Prints the information of the performances of the sorting algorithms given N elements, 
 * where N ranges from 2000 to 50,000.
 * 
 */
void print_sort_data(const vector<int>& input_sizes, ofstream& os) {
    for_each(input_sizes.begin(), input_sizes.end(), print_data_for_n_elems(os));
}


int main () {
  ofstream sort_info;
  const vector<int> info = gen_input_sizes();
  sort_info.open ("sort_data_for_all_algorithms.txt");
  sort_info << "Writing this to a file." << endl;
  sort_info << "another line.\n";
  sort_info.close();
  return 0;
}