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
  std::vector<std::string> vline;

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
    std::string str;
    vline.clear();
    while (!iline.eof()) {
      iline >> str;
      vline.push_back(str);
    }

    bool mustBreak = false;
    for (auto &i : vline) {
      for (auto &j : vline) {

        if (&i != &j) {
          if (i == j) {
            mustBreak = true;
            break;
          } else {
            if (i.length() == j.length()) {

              bool same = true;
              for (auto c : i) {
                std::cout << c << " = " << std::count(i.begin(), i.end(), c) << std::endl;

                if (std::count(i.begin(), i.end(), c) !=
                    std::count(j.begin(), j.end(), c)) {
                  same = false;
                  break;
                }
              }

              if(same) {
                mustBreak = true;
                break;
              }
            }
          }
        }
      }
      if (mustBreak) {
        break;
      }
    }

    if (!mustBreak && vline.size() > 1) {
      ++sum;
    } else {
      std::cout << line << std::endl;
    }
  }

  input_file.close();

  std::cout << sum << std::endl;
}
