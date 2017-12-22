#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

int main() {
  int x = 0, y = 0;
  std::map<int, std::map<int, int>> all;
  int step = 1;
  int c = 1;
  all[0][0] = 1;

  int target = 277678;

  auto check = [&](const int value) {
    // part1
    /* if (value == target) { */
    /*   std::cout << x << ", " << y << std::endl; */

    /*   std::cout << std::abs(x) + std::abs(y) << std::endl; */
    /*   exit(0); */
    /* } */

    int r = 0;
    auto tryadd = [&](const int nx, const int ny) {
      if (all.find(nx) != all.end() && all[nx].find(ny) != all[nx].end()) {
        r += all[nx][ny];
      }
    };

    tryadd(x - 1, y - 1);
    tryadd(x + 1, y - 1);
    tryadd(x - 1, y + 1);
    tryadd(x + 1, y + 1);
    tryadd(x + 1, y);
    tryadd(x - 1, y);
    tryadd(x, y + 1);
    tryadd(x, y - 1);

    all[x][y] = r;

    std::cout << r << std::endl;

    if(r > target)
    {
      std::cout << r << std::endl;

      exit(0);
    }

  };

  while (true) {
    for (int i = 0; i < step; ++i) {
      ++c;
      ++x;
      check(c);
    }
    for (int i = 0; i < step; ++i) {
      ++c;
      ++y;
      check(c);
    }
    ++step;
    for (int i = 0; i < step; ++i) {
      ++c;
      --x;
      check(c);
    }
    for (int i = 0; i < step; ++i) {
      ++c;
      --y;
      check(c);
    }
    ++step;
  }
}
