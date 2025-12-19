#include "../../common_fast.h"

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

struct Context {
    void parse(auto lines) {
    }

    void part1(auto lines) {
        auto result{0_int64};

        int position = 50;

        for (auto line : lines) {
            auto v = line | rv::drop(1) | rv::to_int;
            position += line[0] == 'R' ? v : -v;

            if ((position % 100) == 0) {
                result++;
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};

        int position = 50;

        for (auto line : lines) {
            auto v = line | rv::drop(1) | rv::to_int;

            auto previous = position;

            auto inc = line[0] == 'R' ? 1 : -1;

            while (v > 0) {
                position += inc;
                if ((position % 100) == 0) {
                    result++;
                }
                v--;
            }
        }

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context context;
        context.parse(lines);
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
