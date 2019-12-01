#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto r = 0;
    auto r2 = 0;

    for (auto line : lines) {

        std::istringstream iss{line};
        int v;
        iss >> v;

        r += (v / 3) - 2;

        while (v > 0) {

            v /= 3;
            v -= 2;

            if (v < 0)
                v = 0;

            r2 += v;
        }
    }

    log << r << endl;
    log << r2 << endl;
}

int main() {
    process("input.txt");
    return 0;
}
