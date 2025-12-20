#include "../../common_fast.h"

inline static Uint64 pow10(int n) {
    Uint64 result = 1;

    for (int i = 1; i < 16; ++i) {
        if (i - 1 == n)
            return result;
        result *= 10;
    }

    return result;
}

struct Context {

    void part1(auto lines) {
        auto result{0_int64};

        for (auto line : lines) {
            auto a = std::max_element(line.begin(), line.end() - 1);
            auto b = std::max_element(a + 1, line.end());

            result += (*a - '0') * 10 + (*b - '0');
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};

        for (auto line : lines) {
            auto prev = line.begin() - 1;

            for (int i = 0; i < 12; i++) {
                auto a = std::max_element(prev + 1, line.end() - (12 - i - 1));

                prev = a;

                auto v = pow10(11 - i) * (*a - '0');

                result += v;
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
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
