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

void process(const int input, const int iterations) {
  std::cout << "Input " << input << std::endl;

  std::string current = std::to_string(input);

  for (int i = 0; i < iterations; ++i) {

    char previous_char = 0;
    int count = 1;
    std::string str;
    for (auto c : current) {

      if (c == previous_char) {
        ++count;

      } else {
        if (previous_char) {
          str += std::to_string(count);
          str += previous_char;
        }

        count = 1;
      }

      previous_char = c;
    }

    if (previous_char) {
      str += std::to_string(count);
      str += previous_char;
    }

    current = str;
  }

  std::cout << "Result length " << current.length() << std::endl;
}

int main() {
  process(1, 4);
  process(1321131112, 40);
  process(1321131112, 50);

  return 0;
}
