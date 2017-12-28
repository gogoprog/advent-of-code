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

struct Block {
  std::vector<bool> values;

  explicit Block(std::initializer_list<bool> init) : values{init} {}

  Block() = default;
  ~Block() = default;

  Block(const Block &other) = default;

  Block &operator=(const Block &other) = default;

  void push(const bool v) { values.push_back(v); }
  int size() const { return std::sqrt(values.size()); }

  bool operator==(const Block &other) const { return values == other.values; }

  void log() const {
    int w = size();

    for (int i = 0; i < values.size(); ++i) {
      if ((i % w) == 0) {
        std::cout << std::endl;
      }
      std::cout << (values[i] ? '#' : '.');
    }

    std::cout << std::endl;
  }

  void rotate(const int _size) {
    static std::vector<bool> valcpy;
    valcpy = values;

    assert(_size == size());

    if (_size == 2) {
      values[_size * 0 + 0] = valcpy[_size * 0 + 1];
      values[_size * 0 + 1] = valcpy[_size * 1 + 1];
      values[_size * 1 + 0] = valcpy[_size * 0 + 0];
      values[_size * 1 + 1] = valcpy[_size * 1 + 0];
    } else {
      values[_size * 0 + 0] = valcpy[_size * 0 + 2];
      values[_size * 0 + 1] = valcpy[_size * 1 + 2];
      values[_size * 0 + 2] = valcpy[_size * 2 + 2];
      values[_size * 1 + 0] = valcpy[_size * 0 + 1];
      values[_size * 1 + 2] = valcpy[_size * 2 + 1];
      values[_size * 2 + 0] = valcpy[_size * 0 + 0];
      values[_size * 2 + 1] = valcpy[_size * 1 + 0];
      values[_size * 2 + 2] = valcpy[_size * 2 + 0];
    }
  }

  void flip(const int _size) {
    static std::vector<bool> valcpy;
    valcpy = values;

    if (_size == 2) {
      values[_size * 0 + 0] = valcpy[_size * 0 + 1];
      values[_size * 0 + 1] = valcpy[_size * 0 + 0];
      values[_size * 1 + 0] = valcpy[_size * 1 + 1];
      values[_size * 1 + 1] = valcpy[_size * 1 + 0];
    } else {

      values[_size * 0 + 0] = valcpy[_size * 0 + 2];
      /* values[_size * 0 + 1] = valcpy[_size * 0 + 1]; */
      values[_size * 0 + 2] = valcpy[_size * 0 + 0];
      values[_size * 1 + 0] = valcpy[_size * 1 + 2];
      values[_size * 1 + 2] = valcpy[_size * 1 + 0];
      values[_size * 2 + 0] = valcpy[_size * 2 + 2];
      /* values[_size * 2 + 1] = valcpy[_size * 2 + 1]; */
      values[_size * 2 + 2] = valcpy[_size * 2 + 0];
    }
  }

  void iterate(const std::function<void(Block &, const int)> &func) {
    auto s = size();
    int subsize;

    if ((s % 2) == 0) {
      subsize = 2;
    } else {
      subsize = 3;
    }

    for (int y = 0; y < s; y += subsize) {
      for (int x = 0; x < s; x += subsize) {
        Block subblock;
        subblock.values.reserve(subsize * subsize);

        for (int j = 0; j < subsize; ++j) {
          for (int i = 0; i < subsize; ++i) {
            subblock.push(values[(y + j) * s + x + i]);
          }
        }

        func(subblock, subsize);
      }
    }
  }

  void set(const std::vector<Block> &newblocks) {
    int outsize = std::sqrt(newblocks.size());
    int subsize = newblocks[0].size();
    int s = outsize * subsize;
    values.resize(s * s);
    for (int y = 0; y < outsize; ++y) {
      for (int x = 0; x < outsize; ++x) {
        for (int j = 0; j < subsize; ++j) {
          for (int i = 0; i < subsize; ++i) {
            int index = ((y * subsize + j) * s) + x * subsize + i;
            values[index] = newblocks[y * outsize + x].values[j * subsize + i];
          }
        }
      }
    }
  }

  int count() const {
    int r = 0;
    for (auto v : values) {
      if (v)
        ++r;
    }

    return r;
  }
};

struct Rule {
  Block in;
  Block out;
};

std::string str;
void read(Block &block, std::istringstream &iss) {
  iss >> str;
  for (auto c : str) {
    if (c != '/') {
      block.push(c == '#');
    }
  }
}

void run(const char *filename, const int iterations) {
  std::cout << "\n\nRun " << filename << std::endl;
  std::ifstream f(filename);
  std::string line;

  std::map<int, std::vector<Rule>> rulesMap;

  Block pixels{0, 1, 0, 0, 0, 1, 1, 1, 1};

  while (std::getline(f, line)) {
    std::istringstream iss(line);
    Rule rule;

    read(rule.in, iss);
    iss >> str;
    read(rule.out, iss);

    rulesMap[rule.in.size()].push_back(rule);
  }
  f.close();

  /* pixels.log(); */

  for (int i = 0; i < iterations; ++i) {
    std::vector<Block> newblocks;

    pixels.iterate([&](const Block &_block, const int subsize) {
      auto block = _block;
      auto rules = rulesMap[subsize];
      auto rule =
          std::find_if(rules.begin(), rules.end(), [&](const Rule &rule) {
            if (rule.in == block)
              return true;
            block.flip(subsize);
            if (rule.in == block)
              return true;

            for (int j = 0; j < 4; ++j) {
              block.rotate(subsize);
              if (rule.in == block)
                return true;
              block.flip(subsize);
              if (rule.in == block)
                return true;
              block.flip(subsize);
            }

            return false;
          });

      if (rule == rules.end()) {
        std::cout << "Error!" << std::endl;
      }
      const auto &outblock = (*rule).out;
      newblocks.push_back(outblock);
    });

    pixels.set(newblocks);
    /* pixels.log(); */
  }

  std::cout << pixels.count() << std::endl;
}

int main() {
  run("input.txt", 2);
  run("input2.txt", 5);
  run("input2.txt", 18);
}
