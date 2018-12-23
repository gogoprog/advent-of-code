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

struct VM {
    Registers registers;
    Vector<std::string> sources;
    Program program;
    int *ip;

    void run(const int ipv, const int r0) {
        auto size = program.size();
        int time{0};
        std::fill(registers.begin(), registers.end(), 0);
        registers[0] = r0;
        ip = &registers[ipv];

        bool debugging{false};

        const auto maxTime = 1000000000000;

        while (*ip < size && time < maxTime) {
            auto &line = program[*ip];

            line.operation(registers, line.a, line.b, line.c);

            auto c = *ip;

            if (*ip == 28) {
                debugging = true;
                std::cout << "part1: " << registers[2] << std::endl;
            }

            if (c < size) {
                if (++c >= size) {
                    break;
                }
                (*ip)++;
            }

            /* if (debugging) { */
            /*     log(); */
            /*     system("read -n 1 -s -r -p ..."); */
            /* } */
            ++time;
        }

        if (time < maxTime) {
            std::cout << "Run with " << r0 << std::endl;
            std::cout << "ended: " << time << std::endl;
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

    ull r;
    r = 13522479;
    vm.run(ip, r);
}

int main(int argc, char *argv[]) {
    process(argv[1]);

    return 0;
}
