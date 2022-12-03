#include "../../common.h"

int value(const char c) {

    int addition;

    if (c >= 'a' && c <= 'z') {
        addition = c - 'a' + 1;
    } else {
        addition = c - 'A' + 27;
    }

    return addition;
}

void process(const String filename) {

    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto p1 = 0;
    auto p2 = 0;

    Array<String, 3> lines3;

    Vector<char> badges;
    int i{0};

    for (auto line : lines) {
        Set<int> items;
        auto left = line.substr(0, line.length() / 2);
        auto right = line.substr(line.length() / 2);

        for (auto c : left) {
            if (right.find(c) != String::npos) {
                items.insert(c);
            }
        }

        for (auto c : items) {
            p1 += value(c);
        }

        lines3[i % 3] = line;

        if (i % 3 == 2) {

            for (auto c : lines3[0]) {

                if (lines3[1].find(c) != String::npos && lines3[2].find(c) != String::npos) {
                    p2 += value(c);
                    break;
                }
            }
        }

        ++i;
    }

    log << "Part1: " << p1 << endl;
    log << "Part2: " << p2 << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
