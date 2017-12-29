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

struct Rule {
  int write;
  int move;
  char nextState;
};

struct State {
  std::vector<Rule> rules{2};
};

void run(const char *filename) {
  std::cout << "\n\nRun " << filename << std::endl;
  std::ifstream f(filename);
  std::vector<std::string> lines;
  std::string line, str;
  char c;
  std::map<char, State> states;
  char initial_state;
  int steps;

  {
    std::getline(f, line);
    std::istringstream iss(line);
    iss >> str >> str >> str >> initial_state;
  }

  {
    std::getline(f, line);
    std::istringstream iss(line);
    iss >> str >> str >> str >> str >> str >> steps;
  }
  std::getline(f, line);

  while (std::getline(f, line)) {
    std::istringstream iss(line);
    iss >> str >> str >> c;

    State *state = &states[c];

    for (int i = 0; i < 2; ++i) {
      auto &rule = state->rules[i];
      std::getline(f, line);
      {
        std::getline(f, line);
        std::istringstream iss(line);
        iss >> str >> str >> str >> str >> rule.write;
      }
      {
        std::getline(f, line);
        std::istringstream iss(line);
        iss >> str >> str >> str >> str >> str >> str >> str;
        rule.move = str == "left." ? -1 : 1;
      }
      {
        std::getline(f, line);
        std::istringstream iss(line);
        iss >> str >> str >> str >> str >> rule.nextState;
      }
    }

    std::getline(f, line);
  };

  f.close();

  std::map<int, int> tape;
  int cursor = 0;
  State *current_state = &states[initial_state];

  for (int i = 0; i < steps; ++i) {
    int cv = tape[cursor];
    auto &rule = current_state->rules[cv];

    tape[cursor] = rule.write;
    cursor += rule.move;
    current_state = &states[rule.nextState];
  }

  int r = 0;
  for (auto kv : tape) {
    if (kv.second)
      ++r;
  }

  std::cout << "Result = " << r << std::endl;
}

int main() {
  run("input.txt");
  run("input2.txt");
}
