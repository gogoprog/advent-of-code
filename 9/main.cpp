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

  while (std::getline(input_file, line)) {
    int i = 0;

    while ((i = line.find('!', i)) != std::string::npos) {
      line.erase(i, 2);
    }

    str = "";
    int found = 0;
    int cur = 0;
    int count = 0;
    while ((found = line.find('<', cur)) != std::string::npos) {
      str += line.substr(cur, found - cur);
      cur = line.find('>', found + 1) + 1;
      count += cur - found - 2;
    }

    str += line.substr(cur);

    int level = 0;
    int score = 0;
    for (auto c : str) {
      if (c == '{') {
        ++level;
      }

      if (c == '}') {
        score += level;
        --level;
      }
    }

    std::cout << "Score: " << score << " Removed: " << count << std::endl;
  }

  input_file.close();
}

int main() {
  run("input.txt");
  run("input2.txt");
}
