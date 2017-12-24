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
  int sum = 0;
  std::vector<int> vline;

  // part1
  /* while (getline(input_file, line)) { */
  /*   std::istringstream iline(line); */
  /*   int number; */
  /*   int min = std::numeric_limits<int>::max(); */
  /*   int max = std::numeric_limits<int>::min(); */
  /*   while (!iline.eof()) { */
  /*     iline >> number; */
  /*     min = std::min(min, number); */
  /*     max = std::max(max, number); */
  /*   } */

  /*   sum += max - min; */
  /* } */

  while (getline(input_file, line)) {
    std::istringstream iline(line);
    int number;
    vline.clear();
    while (!iline.eof()) {
      iline >> number;
      vline.push_back(number);
    }

    std::for_each(vline.begin(), vline.end(), [&](int &i) {
      std::for_each(vline.begin(), vline.end(), [&](int &j) {
        if (&i != &j) {
          if (i > j && i % j == 0) {
            /* std::cout << "hello" << std::endl; */
            sum += i / j;
          }
        }
      });
    });
  }

  input_file.close();

  std::cout << sum << std::endl;
}
