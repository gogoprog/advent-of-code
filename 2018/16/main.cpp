#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

using Registers = std::array<int, 4>;

struct VM {};

Registers readRegisters(std::istringstream &iss) {
    Registers result;
    char c;
    iss >> c;

    iss >> result[0];
    iss >> c;
    iss >> result[1];
    iss >> c;
    iss >> result[2];
    iss >> c;
    iss >> result[3];
    iss >> c;

    return result;
}

std::map<std::string, std::function<void(Registers &, int, int, int)>> instructions = {
    {"addr", [](auto &r, auto a, auto b, auto c) { r[c] = r[a] + r[b]; }},
    {"addi", [](auto &r, auto a, auto b, auto c) { r[c] = r[a] + b; }},
    {"mulr", [](auto &r, auto a, auto b, auto c) { r[c] = r[a] * r[b]; }},
    {"muli", [](auto &r, auto a, auto b, auto c) { r[c] = r[a] * b; }},
    {"banr", [](auto &r, auto a, auto b, auto c) { r[c] = r[a] & r[b]; }},
    {"bani", [](auto &r, auto a, auto b, auto c) { r[c] = r[a] & b; }},
    {"borr", [](auto &r, auto a, auto b, auto c) { r[c] = r[a] | r[b]; }},
    {"bori", [](auto &r, auto a, auto b, auto c) { r[c] = r[a] | b; }},
    {"setr", [](auto &r, auto a, auto b, auto c) { r[c] = r[a]; }},
    {"seti", [](auto &r, auto a, auto b, auto c) { r[c] = a; }},
    {"gtir", [](auto &r, auto a, auto b, auto c) { r[c] = (a > r[b]) ? 1 : 0; }},
    {"gtri", [](auto &r, auto a, auto b, auto c) { r[c] = (r[a] > b) ? 1 : 0; }},
    {"gtrr", [](auto &r, auto a, auto b, auto c) { r[c] = (r[a] > r[b]) ? 1 : 0; }},
    {"eqir", [](auto &r, auto a, auto b, auto c) { r[c] = (a == r[b]) ? 1 : 0; }},
    {"eqri", [](auto &r, auto a, auto b, auto c) { r[c] = (r[a] == b) ? 1 : 0; }},
    {"eqrr", [](auto &r, auto a, auto b, auto c) { r[c] = (r[a] == r[b]) ? 1 : 0; }},
};

std::map<int, std::set<std::string>> possibleOpcodes;

void process(const char *file) {
    std::ifstream f(file);
    std::string line;
    std::string str;
    std::vector<std::string> programLines;

    int total{0};

    while (std::getline(f, line)) {
        std::istringstream iss{line};

        iss >> str;

        if (str == "Before:") {
            programLines.clear();
            auto beforeRegisters = readRegisters(iss);
            int opcode, a, b, c;

            {
                std::getline(f, line);
                std::istringstream iss{line};
                iss >> opcode >> a >> b >> c;
            }

            {
                std::getline(f, line);
                std::istringstream iss{line};
                iss >> str;
                auto afterRegisters = readRegisters(iss);
                int count{0};
                std::set<std::string> names;
                auto &possible = possibleOpcodes[opcode];

                for (auto &kv : instructions) {
                    auto testRegisters = beforeRegisters;
                    kv.second(testRegisters, a, b, c);

                    if (std::equal(testRegisters.begin(), testRegisters.end(), afterRegisters.begin())) {
                        ++count;
                        names.insert(kv.first);
                    }
                }

                if (possible.empty()) {
                    possible = names;
                } else {
                    std::set<std::string> result;
                    std::set_intersection(possible.begin(), possible.end(), names.begin(), names.end(),
                                          std::inserter(result, result.begin()));

                    possible = result;
                }

                if (count >= 3) {
                    ++total;
                }
            }
        } else {
            if (line.size()) {
                programLines.push_back(line);
            }
        }
    }

    f.close();

    std::cout << "part1: " << total << std::endl;

    bool allone{false};

    while (!allone) {
        allone = true;

        for (auto &kv : possibleOpcodes) {
            auto &names = kv.second;

            if (names.size() == 1) {

                for (auto &kv2 : possibleOpcodes) {
                    if (kv.first != kv2.first) {
                        kv2.second.erase(*names.begin());
                    }
                }
            } else {
                allone = false;
            }
        }
    }

    std::vector<std::function<void(Registers &, int, int, int)>> funcs(possibleOpcodes.size());

    for (auto &kv : possibleOpcodes) {
        auto &names = kv.second;
        std::cout << kv.first << " => ";
        for (auto name : names) {
            std::cout << name << " | ";
        }

        std::cout << std::endl;

        funcs[kv.first] = instructions[*names.begin()];
    }

    Registers registers{0, 0, 0, 0};
    for (auto &line : programLines) {
        int opcode, a, b, c;
        std::istringstream iss{line};
        iss >> opcode >> a >> b >> c;

        funcs[opcode](registers, a, b, c);
    }

    std::cout << "part2: " << registers[0] << std::endl;
}

int main() {
    /* process("input2.txt"); */
    process("input.txt");

    return 0;
}
