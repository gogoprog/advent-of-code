#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

struct Node {
  std::vector<std::string> strChildren;
  std::vector<Node *> children;
  Node *parent{nullptr};
  std::string name;
  int weight;
  int totalWeight;
  int getTotalWeight() {
    int r = weight;
    int previous = -1;
    bool unbalanced = false;
    for (auto &c : children) {
      int tw = c->getTotalWeight();
      r += tw;

      if (previous != -1) {
        if (previous != tw) {
          unbalanced = true;
        }
      }

      previous = tw;
    }

    if (unbalanced) {
      std::cout << name << " is unbalanced" << std::endl;
      log(0);
      system("read");
    }

    totalWeight = r;
    return r;
  }

  void log(const int level) {
    std::string prefix;
    for (int i = 0; i < level; ++i) {
      prefix += "\t";
    }

    std::cout << prefix << name << " is " << weight << " | " << totalWeight << std::endl;

    for (auto &c : children) {
      c->log(level + 1);
    }
  }
};

int run(const char *filename) {
  std::cout << "\n\nRun " << filename << std::endl;
  std::ifstream input_file(filename);
  std::string line, str;
  char c;
  Node node;
  std::vector<Node> allNodes;

  while (std::getline(input_file, line)) {
    std::istringstream iline(line);
    node.strChildren.clear();

    iline >> node.name;
    iline >> c >> node.weight >> c;

    /* std::cout << node.name << " = " << node.weight << std::endl; */

    if (!iline.eof()) {
      iline >> str;

      while (!iline.eof()) {
        iline >> str;
        if (str.back() == ',') {
          str.erase(str.end() - 1);
        }
        node.strChildren.push_back(str);
      }
    }

    allNodes.push_back(node);
  }

  input_file.close();

  std::cout << "Sorting..." << std::endl;
  for (auto &node : allNodes) {
    for (auto &strName : node.strChildren) {
      auto it =
          std::find_if(allNodes.begin(), allNodes.end(), [&](auto &childNode) {
            return childNode.name == strName;
          });

      auto &childNode = *it;

      node.children.push_back(&childNode);
      childNode.parent = &node;
    }
  }
  std::cout << "Finding root..." << std::endl;
  {
    auto *node = &allNodes[0];

    while (node->parent != nullptr) {
      node = node->parent;
    }

    std::cout << "Root node: " << node->name << std::endl;

    node->getTotalWeight();
  }
}

int main() {
  run("input.txt");
  run("input2.txt");
}
