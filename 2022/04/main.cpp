#include "../../common.h"

using Section = int;
using Range = Pair<Section, Section>;

bool contains(const Range a, const Range b) {

    if (b.first >= a.first && b.second <= a.second) {
        return true;
    }

    return false;
}

bool overlaps(const Range a, const Range b) {
    return a.first <= b.second && b.first <= a.second;
}

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto r = 0;
    auto r2 = 0;

    Range a, b;
    char c;

    for (auto line : lines) {
        InputStringStream iss{line};

        iss >> a.first >> c >> a.second >> c >> b.first >> c >> b.second;

        if (contains(a, b) || contains(b, a)) {
            r++;
        }

        if (overlaps(a, b)) {
            r2++;
        }
    }

    log << "Part1: " << r << endl;
    log << "Part2: " << r2 << endl;
}

int main() {
    process("input.txt");
    return 0;
}
