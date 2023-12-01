#include "../../common.h"

struct Context {
    int compute(const Strings &lines, const Strings &spellings) {
        auto computeLineValue = [&](String line) {
            auto view1 = rv::zip(rv::iota(0), spellings) | rv::transform([&](const auto &pair) {
                             return std::pair{pair.first, line.find(pair.second)};
                         }) |
                         rv::filter([&](auto pair) { return pair.second != String::npos; });

            auto view2 = rv::zip(rv::iota(0), spellings) | rv::transform([&](const auto &pair) {
                             return std::pair{pair.first, line.rfind(pair.second)};
                         }) |
                         rv::filter([&](auto pair) { return pair.second != String::npos; });

            auto min = rs::min(view1, [](auto a, auto b) { return a.second < b.second; });

            auto max = rs::max(view2, [](auto a, auto b) { return a.second < b.second; });

            return ((min.first % 9) + 1) * 10 + (max.first % 9) + 1;
        };

        return rs::fold_left(lines | rv::transform(computeLineValue), 0, std::plus<int>());
    }

    void part1(const Strings &lines) {
        auto result = compute(lines, {"1", "2", "3", "4", "5", "6", "7", "8", "9"});

        log << "Part1: " << result << endl;
    }

    void part2(const Strings &lines) {
        auto result = compute(lines, {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "1", "2",
                                      "3", "4", "5", "6", "7", "8", "9"});
        log << "Part2: " << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        /* Context context; */
        /* context.part1(lines); */
    } {
        Context context;
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
