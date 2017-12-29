#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

struct Node {
  bool visited{false};
  std::vector<int> connections;

  void visit(std::vector<Node> &nodes, const std::vector<int> &path,
             std::vector<std::vector<int>> &groups) {

    if (!visited) {
      bool descending = false;
      for (auto i : connections) {
        auto path2 = path;
        path2.push_back(i);
        if (std::find(path.begin(), path.end(), i) == path.end()) {
          nodes[i].visit(nodes, path2, groups);
          descending = true;
        }
      }

      if (!descending) {
        bool found = false;
        for (auto &group : groups) {
          for (auto p : path) {
            if (std::find(group.begin(), group.end(), p) != group.end()) {
              found = true;
              group.insert(group.end(), path.begin(), path.end());
              std::unique(group.begin(), group.end());
              break;
            }
          }

          if (found) {
            break;
          }
        }

        if (!found) {
          groups.push_back(path);
        }
        visited = true;
      }
    }
  }
};

int run(const char *filename) {
  std::cout << "\n\nRun " << filename << std::endl;
  std::ifstream input_file(filename);
  std::string line, str;
  std::vector<Node> nodes;
  Node curNode;

  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    int id;
    iss >> id;

    curNode.connections.clear();

    while (!iss.eof()) {
      iss >> str;
      iss >> id;
      curNode.connections.push_back(id);
    }

    nodes.push_back(curNode);
  }
  input_file.close();

  int count = 0;
  std::vector<int> path;
  std::vector<std::vector<int>> groups;

  for (int i = 0; i < nodes.size(); ++i) {
    auto &node = nodes[i];

    path = {i};

    node.visit(nodes, path, groups);
  }

  std::cout << "Connected count : " << count << std::endl;
  std::cout << "Groups count : " << groups.size() << std::endl;
}

int main() {
  run("input.txt");
  run("input2.txt");
}
