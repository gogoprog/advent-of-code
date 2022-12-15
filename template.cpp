#include "../../common.h"

struct Context {

    void parse(const Strings &lines) {
        for (auto &line : lines) {
        }
    }

    void part1() {
        auto result{0};

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0};

        log << "Part2: " << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        Context context;
        context.parse(lines);
        context.part1();
    }
    {
        Context context;
        context.parse(lines);
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
