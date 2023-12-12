#include "../../common_fast.h"

struct Context {

    void part1(auto lines) {
        auto result{0};

        auto compute = [](auto line) {

            auto row = line | rv::split_sv(' ') | rv::get0;
            auto groups_v = line | rv::split_sv(' ') | rv::get1 | rv::split_sv(',') | rv::to_ints;

            Vector<int> groups;
            rs::copy(groups_v, std::back_inserter(groups));

            log << row << '\n';

            log << groups << '\n';

            return 0;
        };

        for (auto line : lines) {
            result += compute(line);
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0};

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
    {
        Context context;
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    /* process("input.txt"); */
    return 0;
}
