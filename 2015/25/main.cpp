#include "../../common.h"

void process(const int targetRow, const int targetCol) {
    bool found{false};
    int s{2};
    ull v{20151125};
    int r{0};
    int c{0};
    while (!found) {
        for (int i{0}; i < s; ++i) {
            c = i + 1;
            r = s - i;
            v *= 252533;
            v %= 33554393;

            if (r == targetRow && c == targetCol) {
                log << "part1: " << v << endl;
                return;
            }
        }

        ++s;
    }
}

int main() {
    process(2947, 3029);
    return 0;
}
