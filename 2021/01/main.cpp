#include "../../common.h"

int compute(const Vector<int> lines, const int window_size) {
    auto r = 0;

    for (auto i = window_size; i < lines.size(); ++i) {
        auto previous = 0;
        auto now = 0;

        for (int j = 0; j < window_size; ++j) {
            previous += lines[i - j - 1];
            now += lines[i - j];
        }

        if (now > previous) {
            r++;
        }
    }
    return r;
}

void process(const String filename) {
    log << "Processing " << filename << endl;

    auto lines = getFileLines<int>(filename);

    log << "Part1: " << compute(lines, 1) << endl;
    log << "Part2: " << compute(lines, 3) << endl;
}

int main() {
    process("input.txt");
    return 0;
}
