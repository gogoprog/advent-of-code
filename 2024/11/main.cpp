#include "../../common_fast.h"

int countDigits(Int64 b) {
    Int64 n = 10;
    for (int i = 1; i < 24; ++i) {
        if (n > b) {
            return i;
        }

        n *= 10;
    }
    return 0;
}

struct Context {

    List<Int64> initList;

    void parse(auto lines) {
        rs::copy(lines | rv::split_sv(' ') | rv::to_ints, std::back_inserter(initList));
    }

    void part1() {
        auto result{0_int64};

        auto list = initList;

        for (int i = 0; i < 25; ++i) {

            for (auto it = list.begin(); it != list.end(); ++it) {

                auto &n = *it;

                if (n == 0) {
                    n = 1;
                } else if (int count = countDigits(n); (count & 1) == 0) {
                    Int64 sc = std::pow(10, count / 2);
                    Int64 a = Int64(n / sc);
                    Int64 b = n - (a * sc);
                    list.insert(it, a);
                    auto back = list.insert(it, b);
                    list.erase(it);
                    it = back;
                } else {
                    n *= 2024;
                }
            }
        }

        result = list.size();
        log << "Part1: " << result << endl;
    }

    void part2() {
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
        context.parse(content);
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
