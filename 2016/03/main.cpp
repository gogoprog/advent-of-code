#include "../../common.h"

bool isValid(Vector<int> sides) {
    std::sort(sides.begin(), sides.end());

    return sides[0] + sides[1] > sides[2];
}

void process(const String filename, const bool part2 = false) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    int count{0};
    Vector<Array<int, 3>> input;

    for (auto line : lines) {
        Vector<int> sides;
        InputStringStream iss{line};
        int a, b, c;
        iss >> a >> b >> c;

        sides.push_back(a);
        sides.push_back(b);
        sides.push_back(c);

        input.push_back({a, b, c});

        if (isValid(sides)) {
            ++count;
        }
    }

    log << "part1: " << count << endl;

    if (part2) {

        count = 0;
        for (int p{0}; p < input.size(); p += 3) {
            for (int i{0}; i < 3; ++i) {
                bool r = isValid({input[p][i], input[p + 1][i], input[p + 2][i]});
                if (r) {
                    ++count;
                }
            }
        }

        log << "part2: " << count << endl;
    }
}

int main() {
    process("sample.txt");
    process("input.txt", true);
    return 0;
}
