#include "../../common_fast.h"

ull gcd(ull a, ull b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

ull lcm(ull a, ull b) {
    return (a / gcd(a, b)) * b;
}

struct Context {

    void part1(auto lines) {
        Map<StringView, Pair<StringView, StringView>> map;

        for (auto line : lines | rv::drop(2)) {
            StringView name, left, right;

            name = line.substr(0, 3);

            left = line.substr(7, 3);
            right = line.substr(12, 3);

            map[name] = {left, right};
        }

        auto result{0};

        auto instructions = lines | rv::get0;

        StringView current = "AAA";

        int i = 0;
        while (current != "ZZZ") {

            for (auto c : instructions) {
                switch (c) {
                    case 'L':
                        current = map[current].first;
                        break;
                    case 'R':
                        current = map[current].second;
                        break;
                }

                ++i;
                if (current == "ZZZ") {
                    break;
                }
            }
        }

        result = i;
        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        Map<StringView, Pair<StringView, StringView>> map;

        for (auto line : lines | rv::drop(2)) {
            StringView name, left, right;

            name = line.substr(0, 3);

            left = line.substr(7, 3);
            right = line.substr(12, 3);

            map[name] = {left, right};
        }

        auto instructions = lines | rv::get0;

        auto compute1 = [&](StringView current) {
            ull i = 0;
            ull len = instructions.size();
            while (current[2] != 'Z') {
                auto c = instructions[i % len];

                switch (c) {
                    case 'L':
                        current = map[current].first;
                        break;
                    case 'R':
                        current = map[current].second;
                        break;
                }
                ++i;
            }
            return i;
        };

        ull result = 1;

        for (auto kv : map) {

            if (kv.first[2] == 'A') {

                auto offset = compute1(kv.first);
                result = lcm(result, offset);
            }
        }

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
    /* process("sample.txt"); */
    /* process("sample2.txt"); */
    process("input.txt");
    return 0;
}
