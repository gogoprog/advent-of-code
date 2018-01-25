#include <algorithm>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>
#include <array>

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
  if (!v.empty()) {
    out << '[';
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

template<int inputSize>
struct Config {
  static void build(std::set<std::array<int, inputSize>> &results,
                    const std::vector<int> &sizes,
                    const std::array<int, inputSize> &quantities, const int total,
                    const int current, const int index = 0) {
    if (current < total) {
      for (size_t i = index; i < sizes.size(); ++i) {
        if (quantities[i] < 1) {
          auto new_quantities = quantities;
          new_quantities[i] += 1;
          build(results, sizes, new_quantities, total, current + sizes[i], index + 1);
        }
      }
    } else if (current == total) {
      results.insert(quantities);
    }
  }
};

template<int inputSize>
void processFile(const char *filename, const int total) {
  std::cout << "\nFile " << filename << std::endl;
  std::ifstream f(filename);
  std::string line;
  std::vector<int> sizes;
  int size;

  while (std::getline(f, line)) {
    std::istringstream iss(line);
    iss >> size;
    sizes.push_back(size);
  }

  f.close();

  std::sort(sizes.begin(), sizes.end());

  std::set<std::array<int, inputSize>> configs;
  std::array<int, inputSize> init_quantities;
  std::fill(init_quantities.begin(), init_quantities.end(), 0);

  Config<inputSize>::build(configs, sizes, init_quantities, total, 0);
  std::cout << "Configs: " << configs.size() << std::endl;

  int min = inputSize;
  int count = 0;

  for(auto & c : configs) {
    int s = std::accumulate(c.begin(), c.end(), 0);

    if(s<min) {
      min = s;
      count = 1;
    }
    else if(s == min) {
      ++count;
    }

  }

  std::cout << "Min count: " << count << std::endl;
}

int main() {

  processFile<5>("input.txt", 25);
  processFile<20>("input2.txt", 150);

  return 0;
}
