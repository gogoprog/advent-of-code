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

struct Component {
  int ports[2];
  inline bool supports(const int pins) const {
    return ports[0] == pins || ports[1] == pins;
  }
  inline int getOther(const int previous) const {
    return previous == ports[0] ? ports[1] : ports[0];
  }
};

struct Bridge {
  std::vector<const Component *> components;
  int previous{0};
  std::map<int, bool> used;

  void log() {
    for (const auto &c : components) {
      std::cout << c->ports[0] << '/' << c->ports[1] << " | ";
    }
    std::cout << std::endl;
  }

  void build(const std::vector<Component> &all_components,
             std::vector<Bridge> &bridges) {
    bool ended{true};
    for (int i = 0; i < all_components.size(); ++i) {
      auto &c = all_components[i];
      if (c.supports(previous) && used[i] == false) {
        Bridge bridge_cpy = *this;
        bridge_cpy.previous = c.getOther(previous);
        bridge_cpy.used[i] = true;
        bridge_cpy.components.push_back(&c);
        bridge_cpy.build(all_components, bridges);
        ended = false;
      }
    }

    if (ended) {
      bridges.push_back(*this);
    }
  }

  int strength() const {
    int r = 0;
    for (const auto &c : components) {
      r += c->ports[0];
      r += c->ports[1];
    }
    return r;
  }
};

void getStrongest(const std::vector<Bridge> & bridges) {
  int strongest_value = std::numeric_limits<int>::min();
  for (auto &b : bridges) {
    int str = b.strength();
    if (str > strongest_value) {
      strongest_value = str;
    }
  }

  std::cout << "Strongest " << strongest_value << std::endl;
}

void run(const char *filename) {
  std::cout << "\n\nRun " << filename << std::endl;
  std::ifstream f(filename);
  std::vector<std::string> lines;
  std::string line;
  std::vector<Component> components;
  while (std::getline(f, line)) {
    std::istringstream iss(line);
    Component component;
    char c;
    iss >> component.ports[0];
    iss >> c;
    iss >> component.ports[1];
    components.push_back(component);
  }
  f.close();

  std::vector<Bridge> bridges;
  Bridge root;
  root.build(components, bridges);

  std::cout << "Part1 ";
  getStrongest(bridges);

  std::vector<Bridge> longestBridges;
  int longest = std::numeric_limits<int>::min();
  for (auto &b : bridges) {
    int l = b.components.size();
    if (l > longest) {
      longest = l;
      longestBridges = {b};
    }
    else if(l == longest) {
      longestBridges.push_back(b);
    }
  }

  std::cout << "Part2 ";
  getStrongest(longestBridges);
}

int main() {
  run("input.txt");
  run("input2.txt");
}
