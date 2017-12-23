#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

int main() {
  /* std::vector<int> blocks = {0, 2, 7, 0}; */
  std::vector<int> blocks = {10, 3, 15, 10, 5, 15, 5, 15,
                             9,  2, 5,  8,  5, 2,  3, 6};
  int steps = 0;
  std::vector<std::vector<int>> configs;

  auto addConfig = [&]() -> int {
    auto it = std::find_if(configs.begin(), configs.end(), [&](auto &config) {
      return std::equal(config.begin(), config.end(), blocks.begin());
    });

    if(it != configs.end()){
      return std::distance(configs.begin(), it);
    }

    configs.push_back(blocks);
    return -1;
  };

  auto getMostBlocksIndex = [&]() -> int {
    int r;
    int max = std::numeric_limits<int>::min();

    for (int i = 0; i < blocks.size(); ++i) {
      int b = blocks[i];
      if (b > max) {
        max = b;
        r = i;
      }
    }

    return r;
  };

  auto spreadBlocks = [&](const int index) {
    int count = blocks[index];
    int j = index;
    blocks[index] = 0;

    while (count > 0) {
      ++j;
      j %= blocks.size();
      ++blocks[j];

      --count;
    }
  };

  addConfig();
  int a;

  while (true) {
    int blockIndex = getMostBlocksIndex();
    spreadBlocks(blockIndex);

    ++steps;

    a = addConfig();
    if (a >= 0) {
      break;
    }
  }

  std::cout << configs.size() - a << std::endl;
  std::cout << steps << std::endl;
}
