#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto r = 0;
    auto r2 = 0;

    Set<char> questions;
    Map<char, int> counts;
    int personCount{0};

    auto validate = [&]() {
        r += questions.size();

        for (auto &kv : counts) {
            if (kv.second == personCount) {
                r2++;
            }
        }

        questions.clear();
        counts.clear();
        personCount = 0;
    };

    for (auto &line : lines) {

        if (line != "") {
            personCount++;

            for (auto c : line) {
                questions.insert(c);
                counts[c]++;
            }

        } else {
            validate();
        }
    }

    validate();

    log << "part1: " << r << endl;
    log << "part2: " << r2 << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
