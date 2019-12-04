#include "../../common.h"

inline void compute(int n, int &p1, int &p2) {
    Array<char, 6> str;
    Array<int, 10> adjs;

    std::to_chars(str.begin(), str.end(), n);
    std::fill(adjs.begin(), adjs.end(), 0);

    char previous = 0;

    for (char c : str) {
        if (c < previous) {
            return;
        }

        if (previous == c) {
            adjs[c - '0']++;
        }

        previous = c;
    }

    bool p1d{false};

    for (auto v : adjs) {
        if (v == 1) {
            !p1d && p1++;
            p2++;
            return;
        } else if (v > 1) {
            !p1d && p1++;
            p1d = true;
        }
    }
}

inline void process(const String filename) {
    auto lines = getFileLines(filename);
    auto line = lines[0];
    auto [a, b] = splitNString<int, 2>(line, '-');
    auto p1 = 0;
    auto p2 = 0;

    for (int i = a; i <= b; i++) {
        compute(i, p1, p2);
    }

    log << p1 << endl;
    log << p2 << endl;
}

int main() {
    process("input.txt");
    return 0;
}
