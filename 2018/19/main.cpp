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

template <class K, class V> using Map = std::map<K, V>;
template <class V> using Vector = std::vector<V>;
template <class T> using Grid = Map<int, Map<int, T>>;

using Registers = std::array<int, 6>;

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

struct Line {
    std::function<void(Registers &, int, int, int)> operation;
    int a, b, c;
};

using Program = Vector<Line>;

using ull = unsigned long long;

ull computeDividers(const int target) {
    ull r{0};

    for (ull i{1}; i <= target; ++i) {
        if (target % i == 0) {
            r += i;
        }
    }

    return r;
}

struct VM {
    Registers registers;
    Vector<std::string> sources;
    Program program;
    int *ip;

    void run(const int ipv, const int r0) {
        auto size = program.size();
        std::map<int, bool> hacks;
        std::fill(registers.begin(), registers.end(), 0);
        registers[0] = r0;
        ip = &registers[ipv];

        int last{0};

        while (*ip < size) {
            auto &line = program[*ip];

            /* if (last != registers[0]) { */
            /*     log(); */
            /*     last = registers[0]; */
            /* } */

            if (r0 == 1 && *ip == 11) {
                auto target = registers[2];

                auto part2 = computeDividers(target);

                std::cout << "part2: " << part2 << std::endl;
                exit(0);
            }

            line.operation(registers, line.a, line.b, line.c);

            auto c = *ip;

            if (c < size) {
                if (++c >= size) {
                    break;
                }
                (*ip)++;
            }
            /* system("read -n 1 -s -r -p ..."); */
        }
    }

    void log() {
        std::cout << "-------------------------------------------------------------------" << std::endl;
        int i{0};
        for (auto r : registers) {
            std::cout << 'R' << i++ << ": " << r << std::endl;
        }
        std::cout << "-------------------------------------------------------------------" << std::endl;

        int ri = *ip;
        for (int i{0}; i < sources.size(); ++i) {
            std::cout << (ri == i ? " => " : "    ");
            std::cout << sources[i] << std::endl;
        }
    }
};

void process(const char *file) {
    std::ifstream f(file);
    std::string line;
    std::string str;
    int ip;
    VM vm;

    while (std::getline(f, line)) {
        std::istringstream iss{line};
        iss >> str;

        if (str == "#ip") {
            iss >> ip;
        } else {
            int a, b, c;
            auto func = instructions[str];
            iss >> a >> b >> c;
            vm.program.push_back(Line{func, a, b, c});
            vm.sources.push_back(line);
        }
    }

    f.close();

    vm.run(ip, 0);

    std::cout << "test: " << computeDividers(986) << std::endl;
    std::cout << "part1: " << vm.registers[0] << std::endl;

    vm.run(ip, 1);
}

int main(int argc, char *argv[]) {
    process(argv[1]);

    return 0;
}
