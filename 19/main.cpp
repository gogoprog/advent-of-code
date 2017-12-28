#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <queue>
#include <sstream>
#include <vector>

struct Point {
  int x, y;
  bool operator!=(const Point &other) { return other.x != x || other.y != y; }
  Point &operator+=(const Point &other) {
    x += other.x;
    y += other.y;

    return *this;
  }
};

struct Diagram {
  std::vector<std::string> lines;
  int width, height;
  Point start;

  void init() {
    height = lines.size();
    width = lines[0].length();
    start.y = 0;
    start.x = lines[0].find('|');
  }

  inline char getChar(const int x, const int y) const {

    if (x < 0 || x >= width || y < 0 || y >= height) {
      return ' ';
    }

    return lines[y][x];
  }

  inline char getChar(const Point &p) const { return getChar(p.x, p.y); }

  bool isValid(const Point &p, const char def) const {
    char c = getChar(p);
    return (c >= 'A' && c <= 'Z') || c == def;
  }

  std::string getLetters() const {
    Point pos{start};
    Point dir{0, 1};
    bool end{false};
    std::string result;
    int steps = 0;

    while (!end) {
      ++steps;
      pos += dir;
      char c = getChar(pos);

      if (c >= 'A' && c <= 'Z') {
        result += c;
      } else {
        if (c == '+') {
          if (dir.x == 0) {
            if (isValid({pos.x - 1, pos.y}, '-')) {
              dir = {-1, 0};
            } else {
              dir = {1, 0};
            }
          } else {
            if (isValid({pos.x, pos.y - 1}, '|')) {
              dir = {0, -1};
            } else {
              dir = {0, 1};
            }
          }
        } else {
          if (c != '|' && c != '-') {
            end = true;
          }
        }
      }
    }
    std::cout << "steps " << steps << std::endl;
    return result;
  }
};

int run(const char *filename) {
  std::cout << "\n\nRun " << filename << std::endl;
  std::ifstream f(filename);
  std::string line, str;
  Diagram diagram;
  while (std::getline(f, line)) {
    diagram.lines.push_back(line);
  }
  f.close();

  diagram.init();
  auto r = diagram.getLetters();

  std::cout << "Letters: " << r << std::endl;
}

int main() {
  run("input.txt");
  run("input2.txt");
}
