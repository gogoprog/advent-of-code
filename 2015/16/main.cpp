#include <algorithm>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

using Sue = std::map<std::string, int>;

void processFile(const char *filename) {
  std::cout << "File " << filename << std::endl;
  std::ifstream f(filename);
  std::string line, str;

  std::vector<Sue> sues;

  while (std::getline(f, line)) {
    std::istringstream iss(line);
    Sue sue;
    std::string name;
    char c;
    int v;

    iss >> str >> str;

    while (!iss.eof()) {
      iss >> name >> v >> c;

      name.erase(name.end() - 1);
      sue[name] = v;
    }

    sues.push_back(sue);
  }

  f.close();

  std::map<std::string, int> input = {
      {"children", 3}, {"cats", 7},     {"samoyeds", 2}, {"pomeranians", 3},
      {"akitas", 0},   {"vizslas", 0},  {"goldfish", 5}, {"trees", 3},
      {"cars", 2},     {"perfumes", 1},
  };

  std::map<std::string, std::function<bool(const int a, const int b)>>
      comparators = {
          {"cats", [](auto a, auto b) { return a > b; }},
          {"trees", [](auto a, auto b) { return a > b; }},
          {"pomenarians", [](auto a, auto b) { return a < b; }},
          {"goldfish", [](auto a, auto b) { return a < b; }},
      };

  for (unsigned int i = 0; i < sues.size(); ++i) {

    auto &sue = sues[i];
    bool possible{true};

    for (auto &kv : input) {

      if (sue.find(kv.first) != sue.end()) {
        auto comparator = comparators.find(kv.first);
        if (comparator == comparators.end()) {
          if (sue[kv.first] != kv.second) {
            possible = false;
            break;
          }
        } else {
          if (!(comparator->second)(sue[kv.first], kv.second)) {
            possible = false;
            break;
          }
        }
      }
    }

    if (possible) {
      std::cout << "Sue " << (i + 1) << std::endl;
    }
  }
}

int main() {

  processFile("input2.txt");

  return 0;
}
