#include <algorithm>
#include <bitset>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

std::vector<unsigned char> stringToVector(const std::string str) {
  std::vector<unsigned char> result(str.begin(), str.end());

  result.push_back(17);
  result.push_back(31);
  result.push_back(73);
  result.push_back(47);
  result.push_back(23);

  return result;
}

std::string knot_hash(const std::vector<unsigned char> &input,
                      const int size = 256, const int rounds = 64) {
  std::ostringstream oss;
  auto lengths = input;
  int cur = 0;
  int skip = 0;
  std::vector<int> list(size);
  std::generate(list.begin(), list.end(), [n = 0]() mutable { return n++; });

  for (int r = 0; r < rounds; ++r) {

    for (auto &length : lengths) {
      auto from = list.begin() + cur;
      auto to = from + length;

      if (to <= list.end()) {
        std::reverse(from, to);
      } else {
        std::vector<int> part;
        int to_index = (cur + length) % list.size();
        std::copy(from, list.end(), std::back_inserter(part));
        std::copy(list.begin(), list.begin() + to_index,
                  std::back_inserter(part));
        std::reverse(part.begin(), part.end());

        for (int i = cur; i < list.size(); ++i) {
          list[i] = part[i - cur];
        }
        for (int i = 0; i < to_index; ++i) {
          list[i] = part[i + list.size() - cur];
        }
      }

      cur += length + skip;
      cur %= list.size();
      ++skip;
    }
  }

  if (size == 256) {
    for (int i = 0; i < 16; ++i) {
      int start = i * 16;
      int value = list[start];
      for (int j = 1; j < 16; ++j) {
        int pos = start + j;
        value ^= list[pos];
      }
      oss << std::setfill('0') << std::setw(2) << std::hex << value;
    }
  }

  return oss.str();
}

std::string knot_hash(const std::string &input) {
  return knot_hash(stringToVector(input));
}

using Grid = std::vector<std::bitset<128>>;

void visit(const Grid &grid, Grid &visited_grid, const int row, const int col) {

  if (!grid[row].test(col)) {
    return;
  }

  if (visited_grid[row].test(col)) {
    return;
  }

  visited_grid[row].set(col);

  if (row > 0) {
    visit(grid, visited_grid, row - 1, col);
  }
  if (col > 0) {
    visit(grid, visited_grid, row, col - 1);
  }
  if (row < 127) {
    visit(grid, visited_grid, row + 1, col);
  }
  if (col < 127) {
    visit(grid, visited_grid, row, col + 1);
  }
}

int run(const std::string &input) {
  std::cout << "\n\nRun " << input << std::endl;

  Grid grid;
  grid.resize(128);
  Grid visited_grid;
  visited_grid.resize(128);

  int result = 0;
  for (int row = 0; row < 128; ++row) {
    auto knot = knot_hash(input + "-" + std::to_string(row));
    auto &row_bitset = grid[row];

    for (auto &c : knot) {
      row_bitset <<= 4;
      int v = (c - '0' < 10 ? c - '0' : 10 + c - 'a');
      std::bitset<128> bs(v);
      row_bitset |= bs;
    }

    result += row_bitset.count();
  }

  std::cout << "Result: " << result << std::endl;

  int group = 0;

  for (int r = 0; r < 128; ++r) {
    for (int c = 0; c < 128; ++c) {
      if (!visited_grid[r].test(c) && grid[r].test(c)) {
        ++group;

        visit(grid, visited_grid, r, c);
      }
    }
  }
  std::cout << "Groups: " << group << std::endl;
}

int main() {
  run("flqrgnkx");
  run("nbysizxe");
}
