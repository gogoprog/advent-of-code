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

int compute(const int x, const int y, const int serial) {
    int rackid = x + 10;
    int power = rackid * y;

    power += serial;
    power *= rackid;

    int h = power / 100;
    h %= 10;

    return h - 5;
}

void process(const int serial) {
    std::map<std::string, int> powers;

    for (int y{1}; y <= 300 - 2; ++y) {
        for (int x{1}; x <= 300 - 2; ++x) {
            int total{0};

            for (int i{0}; i < 3; ++i) {
                for (int j{0}; j < 3; ++j) {
                    total += compute(x + i, y + j, serial);
                }
            }

            std::stringstream ss;
            ss << x << ',' << y;

            powers[ss.str()] = total;
        }
    }

    auto it = std::max_element(powers.begin(), powers.end(), [](auto &p1, auto &p2) { return p1.second < p2.second; });

    std::cout << "Part1: " << (*it).first << std::endl;
}

void process2(const int serial) {
    std::string bestid;
    int bestpower{0};

    for (int y{1}; y <= 300; ++y) {
        for (int x{1}; x <= 300; ++x) {
            int maxs = std::min(300 - x, 300 - y);
            int total{0};
            for (int s{1}; s <= maxs; ++s) {

                for (int i{1}; i < s; ++i) {
                    total += compute(x + i, y + s, serial);
                    total += compute(x + s, y + i, serial);
                }

                total += compute(x + s, y + s, serial);

                if (total > bestpower) {
                    std::stringstream ss{};
                    ss << x + 1 << ',' << y + 1 << ',' << s;
                    bestid = ss.str();
                    bestpower = total;
                }
            }
        }
    }

    std::cout << "Part2: " << bestid << std::endl;
}

int main() {
    process(18);
    process(42);
    process(3463);

    process2(18);
    process2(42);
    process2(3463);

    return 0;
}
