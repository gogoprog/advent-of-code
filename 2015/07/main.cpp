#include <algorithm>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

using ushort = unsigned short;

void processFile(const char *filename, const bool override_b = false) {
  std::cout << "File " << filename << std::endl;
  std::ifstream f(filename);
  std::string str, line;

  std::deque<std::string> lines;
  while (std::getline(f, line)) {
    lines.push_back(line);
  }
  f.close();
  std::map<std::string, unsigned short> wires;

  if(override_b)
  wires["b"] = 16076;

  std::map<std::string, std::function<ushort(const ushort a, const ushort b)>>
      operations = {
          {"AND", [](auto a, auto b) { return a & b; }},
          {"OR", [](auto a, auto b) { return a | b; }},
          {"LSHIFT", [](auto a, auto b) { return a << b; }},
          {"RSHIFT", [](auto a, auto b) { return a >> b; }},
      };

  auto getValue = [&](const std::string &str, ushort &out) -> bool {
    if (str[0] >= 'a' && str[0] <= 'z') {
      if (wires.find(str) == wires.end()) {
        return false;
      }
      out = wires[str];
      return true;
    } else {
      out = std::stoi(str);
      return true;
    }
  };

  std::string op;

  while (lines.size() > 0) {
    line = lines.front();
    lines.pop_front();
    std::istringstream iss(line);
    ushort a, b, result;
    iss >> str;

    if (str == "NOT") {
      iss >> str;
      if (!getValue(str, a)) {
        lines.push_back(line);
        continue;
      }
      result = ~a;
    } else {
      if (!getValue(str, a)) {
        lines.push_back(line);
        continue;
      }
      iss >> op;
      if (op != "->") {
        iss >> str;

        if (!getValue(str, b)) {
          lines.push_back(line);
          continue;
        }
        result = operations[op](a, b);

      } else {
        iss >> str;
        if (!override_b || str != "b")
          wires[str] = a;
        continue;
      }
    }

    iss >> str;

    iss >> str;

    if (!override_b || str != "b")
      wires[str] = result;
  }
  f.close();

  std::cout << "a:" << wires["a"] << std::endl;
}

int main() {

  processFile("input2.txt");
  processFile("input.txt");
  processFile("input.txt", true);

  return 0;
}
