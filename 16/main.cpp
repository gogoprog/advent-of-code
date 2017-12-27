#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

int rotate_value(const int i, const int count) {
  int r = i;
  while (r < 0)
    r += count;
  r %= count;
  return r;
}

int run(const char *filename, const int count, const int iterations) {
  std::cout << "\n\nRun " << filename << " x" << iterations << std::endl;
  std::ifstream f(filename);
  std::string inputFileContent((std::istreambuf_iterator<char>(f)),
                               std::istreambuf_iterator<char>());
  std::string line, str;
  std::string dancefloor;
  std::vector<int> positions;
  std::vector<char> char_swaps;

  for (int i = 0; i < count; i++) {
    char c = 'a' + i;
    dancefloor += c;
    positions.push_back(i);
  }

  std::cout << "dancefloor1 = " << dancefloor << std::endl;

  std::istringstream issf(inputFileContent);
  char move;
  char sep;
  while (std::getline(issf, line, ',')) {
    std::istringstream iss(line);

    iss >> move;

    switch (move) {
    case 's':
      int size;
      iss >> size;
      std::rotate(positions.rbegin(), positions.rbegin() + size,
                  positions.rend());
      break;
    case 'x':
      int p1, p2;
      iss >> p1 >> sep >> p2;

      std::iter_swap(positions.begin() + p1, positions.begin() + p2);
      break;
    case 'p':
      char a, b;
      iss >> a >> sep >> b;

      std::iter_swap(dancefloor.begin() + dancefloor.find(a),
                     dancefloor.begin() + dancefloor.find(b));

      break;
    }
  }
  f.close();

  std::cout << "dancefloor2 = " << dancefloor << std::endl;

  for (int p = 0; p < count; ++p) {
    char c = 'a' + p;
    char other = dancefloor[p];
    char_swaps.push_back(other);
  }

  for (int p = 0; p < count; ++p) {
    dancefloor[p] = 'a' + p;
  }

  for (int i = 0; i < iterations; ++i) {

    str = dancefloor;
    for (int p = 0; p < count; ++p) {
      dancefloor[p] = str[positions[p]];
    }

    for (int p = 0; p < count; ++p) {
      dancefloor[p] = char_swaps[dancefloor[p] - 'a'];
    }
  }

  std::cout << "Final: " << dancefloor << std::endl;
}
int main() {
  run("input.txt", 5, 1);
  run("input2.txt", 16, 1);
  run("input.txt", 5, 2);
  run("input2.txt", 16, 1000000000);
}
