#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

template <class T> void logv(const T &v) {
  for (auto i : v)
    std::cout << i << ", ";
  std::cout << std::endl;
}

std::vector<unsigned char> stringToVector(const std::string str) {
  std::vector<unsigned char> result(str.begin(), str.end());

  result.push_back(17);
  result.push_back(31);
  result.push_back(73);
  result.push_back(47);
  result.push_back(23);

  return result;
}

int run(const std::vector<unsigned char> &input, const int size,
        const int rounds = 1) {
  std::cout << "Run size " << size << std::endl;
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

      /* logv(list); */
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
      std::cout << std::setfill('0') << std::setw(2) << std::hex << value;
    }
    std::cout << std::dec << std::endl;
  }

  std::cout << "Multiply 2 first: " << list[0] * list[1] << std::endl;
}

int main() {
  run({3, 4, 1, 5}, 5);
  run({106, 118, 236, 1, 130, 0, 235, 254, 59, 205, 2, 87, 129, 25, 255, 118},
      256);
  run({106, 118, 236, 1, 130, 0, 235, 254, 59, 205, 2, 87, 129, 25, 255, 118},
      256, 64);

  run(stringToVector(""), 256, 64);
  run(stringToVector("AoC 2017"), 256, 64);
  run(stringToVector("106,118,236,1,130,0,235,254,59,205,2,87,129,25,255,118"),
      256, 64);
}
