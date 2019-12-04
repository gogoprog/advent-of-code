#include "../../common.h"

bool isFine(int n, const bool part2) {
    String str = std::to_string(n);

    Map<char, int> adjs;
    char previous = 0;

    for (char c : str) {
        if (previous) {
            if (c < previous) {
                return false;
            }

            if (previous == c) {
                adjs[c]++;
            }
        }

        previous = c;
    }

    for (auto &kv : adjs) {
        if (part2) {
            if (kv.second == 1) {
                return true;
            }
        } else {
            if (kv.second >= 1) {
                return true;
            }
        }
    }

    return false;
}

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto line = lines[0];
    auto [a, b] = splitNString<int, 2>(line, '-');

    auto p1 = 0;
    auto p2 = 0;
    for (int i = a; i <= b; i++) {
        if (isFine(i, false)) {
            p1++;
        }

        if (isFine(i, true)) {
            p2++;
        }
    }

    log << p1 << endl;
    log << p2 << endl;
}

int main() {
    process("input.txt");
    return 0;
}
