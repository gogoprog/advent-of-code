#include <algorithm>
#include <bitset>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

struct FakeSpinlock {
  int pos{0};
  int size{1};
  int last;
  inline void step(const int steps) {
    pos += steps;
    pos %= size;
  }
  inline void insert(const int val) {
    if (pos == 0)
      last = val;
    ++size;
    step(1);
  }

  void solve() { std::cout << last << std::endl; }
};

struct Spinlock {
  int pos{0};
  std::vector<int> buffer{{0}};
  void log() {
    for (int i = 0; i < buffer.size(); ++i) {
      if (i == pos)
        std::cout << '(';
      std::cout << buffer[i];
      if (i == pos)
        std::cout << ')';
      std::cout << ' ';
    }
    std::cout << std::endl;
  }
  inline void step(const int steps) {
    pos += steps;
    pos %= buffer.size();
  }
  inline void insert(const int val) {
    buffer.insert(buffer.begin() + pos + 1, val);
    step(1);
  }
};

void run(const int steps, const int max = 2017) {
  std::cout << "\n\nRun " << steps << std::endl;
  Spinlock spinlock;

  for (int i = 1; i <= max; ++i) {
    spinlock.step(steps);
    spinlock.insert(i);
  }

  spinlock.step(1);
  std::cout << spinlock.buffer[spinlock.pos] << std::endl;
}

int main() {
  run(3);
  run(370);

  // Part 2
  FakeSpinlock spinlock;

  for (int i = 1; i <= 50000000; ++i) {
    spinlock.step(370);
    spinlock.insert(i);
  }

  spinlock.solve();
}
