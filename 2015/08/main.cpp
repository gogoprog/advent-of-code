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

int unescape(const std::string &str) {
  bool escape{false};
  int result = 0;
  int skip = 0;

  for (auto c : str) {

    if (skip > 0) {
      --skip;
      continue;
    }
    if (escape) {
      switch (c) {
      case '\\':
        ++result;
        break;
      case '\"':
        ++result;
        break;
      case 'x':
        skip = 2;
        ++result;
        break;
      }

      escape = false;

    } else {
      if (c == '\\') {
        escape = true;
      } else {
        ++result;
      }
    }
  }

  return result - 2;
}

int escape(const std::string &str) {
  int result = 0;
  for (auto c : str) {
    switch (c) {
    case '\"':
      result += 2;
      break;
    case '\\':
      result += 2;
      break;

    default:
      ++result;
    }
  }

  return result + 2;
}

void processFile(const char *filename) {
  std::cout << "File " << filename << std::endl;
  std::ifstream f(filename);
  std::string line;

  int totalChars = 0;
  int count = 0;
  int count2 = 0;
  while (std::getline(f, line)) {

    totalChars += line.length();
    int unesc = unescape(line);
    count += unesc;

    int esc = escape(line);
    count2 += esc;
  }
  f.close();

  std::cout << "Part1 = " << totalChars - count << std::endl;
  std::cout << "Part2 = " << count2 - totalChars << std::endl;
}

int main() {

  processFile("input.txt");
  processFile("input2.txt");

  return 0;
}
