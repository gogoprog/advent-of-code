#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    auto compute = [&](int sx, int sy) {
        int count{0};

        int h = lines.size();
        int w = lines[0].length();

        int y = 0;
        int x = 0;

        while (y < h) {
            x += sx;
            y += sy;
            x = x % w;

            if (y < h) {
                char c = lines[y][x];
                if (c == '#') {
                    count++;
                }
            }
        }

        return count;
    };

    log << "Part1: " << compute(3, 1) << endl;

    int count2 = compute(3,1) * compute(1,1) * compute(5,1) * compute(7,1) * compute(1,2);
    log << "Part2: " << count2 << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
