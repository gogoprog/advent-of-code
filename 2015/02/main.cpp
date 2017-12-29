#include <algorithm>
#include <fstream>
#include <iostream>
#include <istream>
#include <limits>
#include <sstream>
#include <vector>

int main() {
  std::ifstream f("input.txt");
  std::string line;
  std::vector<int> lengths;

  int total = 0;
  int ribbon = 0;
  while (std::getline(f, line)) {
    int l, w, h;
    char ch;
    std::istringstream iss(line);
    iss >> l >> ch >> w >> ch >> h;

    int a = l * w;
    int b = w * h;
    int c = h * l;

    total += 2 * l * w + 2 * w * h + 2 * h * l;
    total += std::min(a, std::min(b, c));

    lengths = {w, h, l};
    std::sort(lengths.begin(), lengths.end());

    ribbon += lengths[0] * 2 + lengths[1] * 2;
    ribbon += w * l * h;
  };
  f.close();

  std::cout << "Part1: " << total << std::endl;
  std::cout << "Part2: " << ribbon << std::endl;

  return 0;
}
