#include <algorithm>
#include <fstream>
#include <iostream>
#include <istream>
#include <limits>

int main() {
  std::ifstream f("input.txt");
  std::string line;
  std::getline(f, line);
  f.close();

  std::cout << "Part1: "
            << std::count(line.begin(), line.end(), '(') -
                   std::count(line.begin(), line.end(), ')')
            << std::endl;

  int floor = 0;
  for (auto it = line.begin(); it != line.end(); ++it) {
    char c = *it;
    if (c == '(')
      ++floor;
    else
      --floor;

    if (floor == -1) {
      std::cout << "Part2: " << std::distance(line.begin(), it) + 1 << std::endl;
      return 0;
    }
  }

  return 0;
}
