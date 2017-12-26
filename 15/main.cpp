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

int run(const int gen_a, const int gen_b, const int iterations, const int mod_a,
        const int mod_b) {
  std::cout << "\n\nRun " << gen_a << ", " << gen_b << std::endl;
  int pairs = 0;

  long long val_a = gen_a, val_b = gen_b;
  long long cp_a, cp_b;

  for (int i = 0; i < iterations; ++i) {

    do {
      val_a = (val_a * 16807) % 2147483647;
    } while ((val_a % mod_a) != 0);

    do {
      val_b = (val_b * 48271) % 2147483647;
    } while ((val_b % mod_b) != 0);

    cp_a = val_a & ((1 << 16) - 1);
    cp_b = val_b & ((1 << 16) - 1);

    if (cp_a == cp_b) {
      ++pairs;
    }
  }

  std::cout << "Matching pairs: " << pairs << std::endl;
}

int main() {
  run(65, 8921, 40000000, 1, 1);
  run(679, 771, 40000000, 1, 1);
  run(65, 8921, 5000000, 4, 8);
  run(679, 771, 5000000, 4, 8);
}
