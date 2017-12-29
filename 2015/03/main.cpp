#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

struct Vec2 {
  int x{0}, y{0};
};

int main() {
  std::ifstream f("input.txt");
  std::string line;
  std::getline(f, line);
  f.close();

  std::map<char, std::function<void(Vec2 & v)>> moveFuncs = {
      {'^', [](auto &v) { ++v.y; }},
      {'<', [](auto &v) { --v.x; }},
      {'>', [](auto &v) { ++v.x; }},
      {'v', [](auto &v) { --v.y; }},
  };

  {
    Vec2 pos;
    std::map<int, std::map<int, bool>> houses;
    houses[0][0] = true;
    int count = 1;
    for (auto c : line) {
      moveFuncs[c](pos);
      if (!houses[pos.x][pos.y]) {
        ++count;
        houses[pos.x][pos.y] = true;
      }
    }

    std::cout << "Part1: " << count << std::endl;
  }

  {
    Vec2 poss[] = {{0,0}, {0,0}};
    std::map<int, std::map<int, bool>> houses;
    houses[0][0] = true;
    int count = 1;
    int posIndex = 0;
    for (auto c : line) {
      auto & pos =  poss[posIndex];
      moveFuncs[c](pos);
      if (!houses[pos.x][pos.y]) {
        ++count;
        houses[pos.x][pos.y] = true;
      }
      posIndex = 1 - posIndex;
    }

    std::cout << "Part2: " << count << std::endl;
  }

  return 0;
}
