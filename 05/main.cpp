#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

int main() {
  std::ifstream input_file("input.txt");
  std::string line;
  std::vector<int> values;

  while (getline(input_file, line)) {
    std::istringstream issline(line);
    int number;
    issline >> number;
    values.push_back(number);
    std::cout << number << std::endl;
  }

  input_file.close();

  /* values = {0, 3, 0, 1, -3}; */

  int cursor = 0;
  int steps = 0;

  while (cursor < values.size() and cursor >= 0) {

    /* std::cout << values[0] << " " << values[1] << " " << values[2] << " " <<
     * values[3] << " " << values[4] << std::endl; */

    int prev = cursor;
    int val = values[cursor];
    cursor += val;
    if (val >= 3) {
      values[prev] = values[prev] - 1;
    } else {
      values[prev] = values[prev] + 1;
    }

    ++steps;
  }

  std::cout << steps << std::endl;
}
