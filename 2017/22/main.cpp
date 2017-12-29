#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>

struct Vec2 {
  int x, y;
};

enum class Flag : char { CLEAN = 0, WEAKENED, INFECTED, FLAGGED };
const char displays[] = ".W#F";

void run(const char *filename, const int bursts) {
  std::cout << "\n\nRun " << filename << std::endl;
  std::ifstream f(filename);
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(f, line)) {
    lines.push_back(line);
  }
  f.close();
  std::map<int, std::map<int, Flag>> infecteds;

  for (int j = 0; j < lines.size(); ++j) {
    auto &line = lines[j];
    int y = j - lines.size() / 2;
    for (int i = 0; i < line.length(); ++i) {
      int x = i - lines.size() / 2;
      infecteds[x][y] = (line[i] == '#' ? Flag::INFECTED : Flag::CLEAN);
    }
  }

  Vec2 pos{0, 0};
  Vec2 dir{0, 1};
  int dir_index = 2;
  Vec2 directions[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  int count = 0;

  for (int b = 0; b < bursts; ++b) {

    Flag &flag = infecteds[pos.x][pos.y];

    switch(flag) {
      case Flag::CLEAN:
        ++dir_index;
        break;
      case Flag::WEAKENED:
        break;
      case Flag::INFECTED:
        --dir_index;
        break;
      case Flag::FLAGGED:
        dir_index += 2;
        break;
    }

    while (dir_index < 0)
      dir_index += 4;
    dir_index %= 4;

    flag = static_cast<Flag>((static_cast<char>(flag) + 1) % 4);

    if (flag == Flag::INFECTED)
      ++count;

    auto &dir = directions[dir_index];

    pos.x += dir.x;
    pos.y += dir.y;

    /* std::cout << "=" << std::endl; */
    /* for (int y = -10; y < 10; ++y) { */
    /*   for (int x = -10; x < 10; ++x) { */
    /*     Flag flag = infecteds[x][y]; */
    /*     bool current = (x == pos.x && y == pos.y); */
    /*     std::cout << (current ? '[' : ' '); */
    /*     std::cout << displays[static_cast<char>(flag)]; */
    /*     std::cout << (current ? ']' : ' '); */
    /*   } */
    /*   std::cout << std::endl; */
    /* } */
  }

  std::cout << "Infections " << count << std::endl;
}

int main() {
  run("input.txt", 100);
  run("input2.txt", 10000000);
}
