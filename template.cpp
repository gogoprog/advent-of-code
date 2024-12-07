#include "../../common_fast.h"

struct Context {

    void part1(auto lines) {
        auto result{0_int64};

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context context;
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
