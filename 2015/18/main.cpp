#include <algorithm>
#include <array>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
  if (!v.empty()) {
    out << '[';
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

template <int inputSize>
void processFile(const char *filename, const int steps,
                 const bool forceCorners = false) {
  std::cout << "\nFile " << filename << std::endl;
  std::ifstream f(filename);
  std::string line;

  std::map<int, std::map<int, bool>> state, nextState;

  int j = 0;
  while (std::getline(f, line)) {
    for (unsigned int i = 0; i < line.length(); ++i) {
      state[j][i] = line[i] == '#';
    }
    ++j;
  }

  f.close();

  auto forceCornersFunc = [&]() {
    if (forceCorners) {
      state[0][0] = true;
      state[0][inputSize - 1] = true;
      state[inputSize - 1][0] = true;
      state[inputSize - 1][inputSize - 1] = true;
    }
  };

  forceCornersFunc();

  for (int s = 0; s < steps; ++s) {

    for (unsigned int i = 0; i < inputSize; ++i) {
      for (unsigned int j = 0; j < inputSize; ++j) {
        int surround = 0;

        surround += state[j + 1][i + 1] ? 1 : 0;
        surround += state[j - 1][i + 1] ? 1 : 0;
        surround += state[j - 1][i - 1] ? 1 : 0;
        surround += state[j + 1][i - 1] ? 1 : 0;
        surround += state[j + 1][i] ? 1 : 0;
        surround += state[j][i + 1] ? 1 : 0;
        surround += state[j - 1][i] ? 1 : 0;
        surround += state[j][i - 1] ? 1 : 0;

        if (state[j][i]) {
          nextState[j][i] = (surround == 2 or surround == 3) ? 1 : 0;
        } else {
          nextState[j][i] = (surround == 3) ? 1 : 0;
        }
      }
    }
    state = nextState;
    forceCornersFunc();
  }

  int count = 0;

  for (unsigned int i = 0; i < inputSize; ++i) {
    for (unsigned int j = 0; j < inputSize; ++j) {
      if (state[j][i]) {
        ++count;
      }
    }
  }

  std::cout << "Lights on = " << count << std::endl;
}

int main() {

  processFile<6>("input.txt", 4);
  processFile<100>("input2.txt", 100);
  processFile<100>("input2.txt", 100, true);

  return 0;
}
