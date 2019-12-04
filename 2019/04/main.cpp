#include "../../common.h"

bool isFine(int n) {
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
        if (kv.second == 1) {
            return true;
        }
    }

    return false;
}

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto line = lines[0];
    auto r = 0;
    auto args = splitString<int>(line, '-');
    int a = args[0];
    int b = args[1];

    for (int i = a; i <= b; i++) {
        if (isFine(i)) {
            r++;
        }
    }

    log << r << endl;
}

int main() {
    process("input.txt");
    return 0;
}
