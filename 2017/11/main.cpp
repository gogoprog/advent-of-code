#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

int run(const std::string &input) {
  std::cout << "\n\nRun " << std::endl;
  std::istringstream iss(input);
  std::string str;

  int x = 0;
  int y = 0;
  int z = 0;

  int highest = std::numeric_limits<int>::min();

  std::map<std::string, std::function<void()>> moveFuncs = {
      {"n",
       [&]() {
         ++y;
         --z;
       }},
      {"ne",
       [&]() {
         ++x;
         --z;
       }},
      {"se",
       [&]() {
         --y;
         ++x;
       }},
      {"s",
       [&]() {
         --y;
         ++z;
       }},
      {"sw",
       [&]() {
         --x;
         ++z;
       }},
      {"nw",
       [&]() {
         --x;
         ++y;
       }},
  };

  while (std::getline(iss, str, ',')) {
    moveFuncs[str]();
    int distance = (std::abs(x) + std::abs(y) + std::abs(z)) / 2;
    highest = std::max(highest, distance);
  }

  int result = (std::abs(x) + std::abs(y) + std::abs(z)) / 2;

  std::cout << "result= " << result << std::endl;
  std::cout << "highest= " << highest << std::endl;
}

int main() {
  std::ifstream t("input.txt");
  std::string inputFileContent((std::istreambuf_iterator<char>(t)),
                               std::istreambuf_iterator<char>());
  run("ne,ne,ne");
  run("ne,ne,sw,sw");

  inputFileContent.erase(inputFileContent.end() - 1);
  run(inputFileContent);
}
