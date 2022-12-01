#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto r = 0;

    log << r << endl;

    bool mustcreate{true};
    Vector<int> cals;

    for (auto line : lines) {
        InputStringStream iss{line};

        if (line != "") {
            if (mustcreate) {
                mustcreate = false;
                cals.push_back(0);
            }
            int cal;
            iss >> cal;
            cals.back() += cal;
        } else {
            mustcreate = true;
        }
    }

    auto result = std::max_element(cals.begin(), cals.end());

    log << "Part1: " << *result << endl;

    std::sort(cals.begin(), cals.end(), std::greater<int>());

    auto sum = cals[0] + cals[1] + cals[2];

    log << "Part2: " << sum << endl;
}

int main() {
    process("input.txt");
    return 0;
}
