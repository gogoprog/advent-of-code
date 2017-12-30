#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

bool isNice(const std::string &input) {
  static std::vector<std::string> forbidden = {"ab", "cd", "pq", "xy"};
  static std::string vowels = "aeiou";

  char previous = 0;
  std::string two;

  bool has2consecutive{false};
  int vowels_count = 0;

  for (auto c : input) {

    if (previous) {
      two = previous;
      two += c;

      if (std::find(forbidden.begin(), forbidden.end(), two) !=
          forbidden.end()) {
        return false;
      }
    }

    if (c == previous) {
      has2consecutive = true;
    }

    if (vowels.find(c) != std::string::npos) {
      ++vowels_count;
    }

    previous = c;
  }

  return has2consecutive && vowels_count >= 3;
}

bool isNice2(const std::string &input) {
  bool condition1{false};
  bool condition2{false};

  for (int i = 0; i < input.length(); ++i) {
    if (i > 0 && !condition1) {
      std::string sub = input.substr(i - 1, 2);
      if (input.find(sub, i + 1) != std::string::npos) {
        condition1 = true;
      }
    }

    if (i > 1) {
      if (input[i - 2] == input[i]) {
        condition2 = true;
      }
    }
  }

  return condition1 && condition2;
}

void processFile(const char *filename) {
  std::ifstream f(filename);
  std::string line;
  int count = 0;
  int count2 = 0;
  while (std::getline(f, line)) {
    if (isNice(line)) {
      ++count;
    }
    if (isNice2(line)) {
      ++count2;
    }
  }
  f.close();

  std::cout << "Part1: " << count << std::endl;
  std::cout << "Part1: " << count2 << std::endl;
}

int main() {

  std::cout << isNice("ugknbfddgicrmopn") << std::endl;
  std::cout << isNice("jchzalrnumimnmhp") << std::endl;
  std::cout << isNice("haegwjzuvuyypxyu") << std::endl;
  std::cout << isNice("dvszwmarrgswjxmb") << std::endl;
  std::cout << isNice2("qjhvhtzxzqqjkmpb") << std::endl;
  std::cout << isNice2("uurcxstgmygtbstg") << std::endl;

  processFile("input.txt");

  return 0;
}
