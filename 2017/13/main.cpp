#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

struct Layer {
  int depth{0};
};

int run(const char *filename) {
  std::cout << "\n\nRun " << filename << std::endl;
  std::ifstream input_file(filename);
  std::string line, str;
  std::vector<Layer> layers;

  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    int index;
    int d;

    iss >> index >> str >> d;

    if (layers.size() <= index) {
      layers.resize(index + 1);
    }

    layers[index].depth = d;
  }
  input_file.close();

  int delay = 0;
  bool success{false};

  while (!success) {

    int pos = 0;
    bool hit{false};

    while (pos < layers.size()) {
      auto &layer = layers[pos];

      if (layer.depth != 0) {
        int scanPos = (pos + delay) % (layer.depth * 2 - 2);

        if (scanPos == 0) {
          hit = true;
          ++delay;
          break;
        }
      }

      ++pos;
    }

    if (!hit) {
      success = true;
    }
  }

  std::cout << "Delay: " << delay << std::endl;
}

int main() {
  run("input.txt");
  run("input2.txt");
}
