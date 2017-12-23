#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

int run(const char *filename) {
  std::cout << "\n\nRun " << filename << std::endl;
  std::ifstream input_file(filename);
  std::string line, str;
  std::map<std::string, int> registers;

  std::map<std::string, std::function<bool(const int, const int)>> condFuncs = {
      {"==", [](const int a, const int b) { return a == b; }},
      {"!=", [](const int a, const int b) { return a != b; }},
      {">=", [](const int a, const int b) { return a >= b; }},
      {"<=", [](const int a, const int b) { return a <= b; }},
      {">", [](const int a, const int b) { return a > b; }},
      {"<", [](const int a, const int b) { return a < b; }},
  };

  while (std::getline(input_file, line)) {
    std::istringstream iline(line);
    std::string targetReg, instruction, condReg, condOp;
    int targetValue, condValue;

    iline >> targetReg >> instruction >> targetValue >> str >> condReg >>
        condOp >> condValue;

    if (registers.find(condReg) == registers.end()) {
      registers[condReg] = 0;
    }
    if (registers.find(targetReg) == registers.end()) {
      registers[targetReg] = 0;
    }

    auto func = condFuncs[condOp];

    if (func(registers[condReg], condValue)) {
      if (instruction == "inc") {
        registers[targetReg] += targetValue;
      } else {
        registers[targetReg] -= targetValue;
      }
    }
  }

  input_file.close();

  auto result = std::min_element(registers.begin(), registers.end());
  std::cout << "Minimum is " << result->second << std::endl;
}

int main() {
  run("input.txt");
  run("input2.txt");
}
