#include "../../common.h"

struct Context {
    Strings lines;

    void parse(const Strings &lines) {
        this->lines = lines;
    }

    int compute(Strings spellings) {
        auto result{0};

        auto computeLineValue = [&](String line) {
            auto view1 = rv::zip(rv::iota(0), spellings) | rv::transform([&](auto pair) {
                             return std::pair{(pair.first % 9) + 1, line.find(pair.second)};
                         }) |
                         rv::filter([&](auto pair) { return pair.second != String::npos; });

            auto view2 = rv::zip(rv::iota(0), spellings) | rv::transform([&](auto pair) {
                             return std::pair{(pair.first % 9) + 1, line.rfind(pair.second)};
                         }) |
                         rv::filter([&](auto pair) { return pair.second != String::npos; });

            auto min = rs::min(view1, [](auto a, auto b) { return a.second < b.second; });

            auto max = rs::max(view2, [](auto a, auto b) { return a.second < b.second; });

            return min.first * 10 + max.first;
        };

        return rs::fold_left(lines | rv::transform(computeLineValue), 0, std::plus<int>());
    }

    void part1() {
        auto result = compute({"1", "2", "3", "4", "5", "6", "7", "8", "9"});

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result = compute({"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "1", "2", "3",
                               "4", "5", "6", "7", "8", "9"});
        log << "Part2: " << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        /* Context context; */
        /* context.parse(lines); */
        /* context.part1(); */
    } {
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
