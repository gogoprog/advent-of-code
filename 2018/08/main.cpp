#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

struct Node {
    std::vector<int> metadata;
    std::vector<Node> children;

    int checksum() {
        int sum = std::accumulate(metadata.begin(), metadata.end(), 0);

        for (auto &child : children) {
            sum += child.checksum();
        }

        return sum;
    }

    int value() {
        if (children.empty()) {
            return std::accumulate(metadata.begin(), metadata.end(), 0);
        } else {
            int sum{0};
            for (auto m : metadata) {
                if (m - 1< children.size()) {
                    sum += children[m - 1].value();
                }
            }
            return sum;
        }
    }
};

void readNode(Node &node, std::istringstream &iss) {

    int childrenCount;
    int metadataCount;

    iss >> childrenCount >> metadataCount;

    for (int c{0}; c < childrenCount; ++c) {
        node.children.push_back(Node{});

        auto &child = node.children.back();

        readNode(child, iss);
    }
    for (int m{0}; m < metadataCount; ++m) {
        int data;
        iss >> data;
        node.metadata.push_back(data);
    }
}

void process(const char *file) {
    std::ifstream f(file);
    std::string line, str;
    std::map<char, std::vector<char>> requirements{};
    std::set<char> steps;

    std::getline(f, line);
    f.close();
    std::istringstream iss{line};

    Node rootNode;

    readNode(rootNode, iss);

    std::cout << rootNode.checksum() << std::endl;
    std::cout << rootNode.value() << std::endl;
}

int main() {
    process("input2.txt");
    process("input.txt");

    return 0;
}
