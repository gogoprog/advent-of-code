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

int countNumbers(const std::string &str) {
  int total = 0;

  std::string current;

  for (size_t i = 0; i < str.length(); ++i) {
    char c = str[i];

    if (c >= '0' && c <= '9') {
      if (current.length() == 0) {
        if (i > 0 && str[i - 1] == '-') {
          current = '-';
        }
      }

      current += c;
    } else {
      if (current.length() > 0) {
        total += std::stoi(current);

        current.clear();
      }
    }
  }

  return total;
}

void removeRed(std::string &str) {
  std::string result;
  size_t red_pos = 0;
  size_t start_pos = 0;
  size_t end_pos = 0;

  while ((red_pos = str.find("red", start_pos)) != std::string::npos) {

    if (str[red_pos - 1] == '\"' && str[red_pos + 3] == '\"' &&
        (str[red_pos - 2] == ',' || str[red_pos - 2] == '[')) {
      start_pos = red_pos - 2;
      end_pos = red_pos + 4;

    } else {

      int level = 0;
      for (int i = red_pos - 1; i >= 0; --i) {
        if (str[i] == '}')
          ++level;
        if (str[i] == '{') {
          if (level == 0) {
            start_pos = i;
            break;
          }
          --level;
        }
      }

      level = 0;
      for (size_t i = red_pos + 3; i < str.length(); ++i) {
        if (str[i] == '{')
          ++level;
        if (str[i] == '}') {
          if (level == 0) {
            end_pos = i + 1;
            break;
          }
          --level;
        }
      }
    }

    str.erase(start_pos, end_pos - start_pos);
  }
}

void processFile(const char *filename) {
  std::cout << "File " << filename << std::endl;
  std::ifstream f(filename);
  std::string line;

  while (std::getline(f, line)) {
    removeRed(line);
    int r = countNumbers(line);
    std::cout << "Result: " << r << std::endl;
  }
}

int main() {

  processFile("input.txt");
  processFile("input2.txt");

  return 0;
}
