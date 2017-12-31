#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

void processFile(const char *filename) {
  std::ifstream f(filename);
  std::string line, str;
  char c;
  std::vector<std::vector<int>> grid;
  grid.resize(1000);

  for (auto &line : grid) {
    line.resize(1000);
  }

  int from_x, from_y, to_x, to_y;

  while (std::getline(f, line)) {
    std::istringstream iss(line);
    bool value;
    bool toggle{false};

    iss >> str;

    if (str == "turn") {
      iss >> str;
      value = (str == "on");
    } else {
      toggle = true;
    }

    iss >> from_x;
    iss >> c;
    iss >> from_y;

    iss >> str;

    iss >> to_x;
    iss >> c;
    iss >> to_y;

    for (int x = from_x; x <= to_x; ++x) {
      for (int y = from_y; y <= to_y; ++y) {
        auto &v = grid[x][y];
        if (toggle) {
          v += 2;
        } else {
          v += value ? 1 : -1;
          v = std::max(0, v);
        }
      }
    }
  }
  f.close();

  int count = 0;
  for (int x = 0; x < 1000; ++x) {
    for (int y = 0; y < 1000; ++y) {
      count += grid[x][y];
    }
  }

  std::cout << "Count " << count << std::endl;
}

int main() {

  processFile("input.txt");

  return 0;
}
