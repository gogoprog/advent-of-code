#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

using Recipe = std::vector<int8_t>;

struct Elf {
    int p{0};
};

std::ostream &operator<<(std::ostream &out, const std::vector<int8_t> &v) {
    if (!v.empty()) {
        for (auto vv : v) {
            out << int(vv) << ' ';
        }
    }
    return out;
}

void fillRecipe(Recipe &result, int n) {
    if (n == 0) {
        result.push_back(0);
        return;
    }
    std::list<int8_t> addition;
    while (n != 0) {
        int8_t i = (n % 10);
        addition.push_front(i);
        n /= 10;
    }

    result.insert(result.end(), addition.begin(), addition.end());
}

void process(const int first, const int size, const int count) {

    std::vector<Elf> elves(2);
    Recipe recipe{};
    Recipe targetRecipe{};

    bool part2{size == -1};

    if (part2) {
        fillRecipe(targetRecipe, count);
    }

    fillRecipe(recipe, first);

    elves[0].p = 0;
    elves[1].p = 1;

    int offset{0};

    while (part2 || recipe.size() + offset < count + size) {

        int result{0};

        for (auto &elf : elves) {
            result += recipe[elf.p];
        }

        fillRecipe(recipe, result);

        if (part2) {
            if (std::equal(recipe.end() - targetRecipe.size(), recipe.end(), targetRecipe.begin())) {
                std::cout << "part2: " << recipe.end() - targetRecipe.size() - recipe.begin() << std::endl;
                break;
            }
            if (std::equal(recipe.end() - targetRecipe.size() - 1, recipe.end() - 1, targetRecipe.begin())) {
                std::cout << "part2: " << recipe.end() - targetRecipe.size() - recipe.begin() - 1 << std::endl;
                break;
            }
        }

        for (auto &elf : elves) {
            elf.p += recipe[elf.p] + 1;
            elf.p %= recipe.size();
        }
    }

    if (!part2) {
        std::cout << "part1: ";
        for (auto it = recipe.begin() + count; it != recipe.begin() + count + size; ++it) {
            std::cout << int(*it);
        }
        std::cout << std::endl;
    }
}

int main() {
    process(37, 10, 9);
    process(37, 10, 5);
    process(37, 10, 18);
    process(37, 10, 2018);
    process(37, 10, 327901);
    process(37, -1, 51589);
    process(37, -1, 01245);
    process(37, -1, 92510);
    process(37, -1, 59414);
    process(37, -1, 327901);

    return 0;
}
