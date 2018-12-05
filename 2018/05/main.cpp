#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

int process(const std::string &input, const char forbidden = 0) {
    std::string current = input;
    bool found{true};

    if (forbidden) {
        current.erase(std::remove(current.begin(), current.end(), forbidden), current.end());
        current.erase(std::remove(current.begin(), current.end(), forbidden + 'A' - 'a'), current.end());
    }

    while (found) {
        found = false;
        for (int c{0}; c < current.length() - 1; ++c) {
            char currentChar = current[c];
            char nextChar = current[c + 1];
            if (currentChar == nextChar + 'A' - 'a' or currentChar == nextChar - 'A' + 'a') {
                current.erase(c, 2);
                found = true;
            }
        }
    }

    return current.length();
}

int main() {
    std::ifstream ifs("input.txt");
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    content.erase(content.length() - 1);
    std::cout << content << std::endl;
    /* process("dabAcCaCBAcCcaDA"); */
    std::cout << "part1: " << process(content) << std::endl;

    std::map<char, int> results;

    for (char c{'a'}; c <= 'z'; ++c) {
        results[c] = process(content, c);
    }

    auto it = std::min_element(results.begin(), results.end());
    std::cout << "part2: " << (*it).first << " -> " << (*it).second << std::endl;

    return 0;
}
