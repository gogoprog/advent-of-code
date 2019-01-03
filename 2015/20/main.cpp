#include <algorithm>
#include <array>
#include <cmath>
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

void part1(const int target) {
    int h{0};

    while (true) {
        int r{0};

        auto sqrh = (int)sqrt(h);

        for (int i{1}; i <= sqrh; ++i) {
            if (h % i == 0) {
                r += i * 10;
                r += (h / i) * 10;
            }
        }

        if (r >= target) {
            std::cout << "part1: " << h << " = " << r << std::endl;
            return;
        }
        ++h;
    }
}

void part2(const int target) {
    std::map<int, int> houses;

    int e{1};
    while (e < 1000000) {
        int p{e * 11};

        for (int h{e}, i{0}; i < 50; h += e, ++i) {
            houses[h] += p;
        }

        ++e;
    }

    int best;

    for (int i{1}; i < 1000000; ++i) {
        auto hh = houses[i];

        if (hh >= target) {
            best = i;
            break;
        }
    }

    std::cout << "part2: " << best << std::endl;
}

int main() {
    /* part1(33100000); */
    /* part2(13000); */
    part2(60);
    part2(33100000);

    return 0;
}
