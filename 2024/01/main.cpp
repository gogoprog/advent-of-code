#include "../../common_fast.h"

struct Context {

    void part1(auto lines) {
        auto result{0};

        Vector<int> a;
        Vector<int> b;

        for (auto line : lines) {

            auto v1 = line | rv::split_string_view(' ') | rv::filter_empty | rv::get0 | rv::to_int;
            auto v2 = line | rv::split_string_view(' ') | rv::filter_empty | rv::get1 | rv::to_int;

            a.push_back(v1);
            b.push_back(v2);
        }

        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end());

        for (int i = 0; i < a.size(); i++) {
            result += std::abs(a[i] - b[i]);
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0};

        Vector<int> a;
        Vector<int> b;

        for (auto line : lines) {

            auto v1 = line | rv::split_string_view(' ') | rv::filter_empty | rv::get0 | rv::to_int;
            auto v2 = line | rv::split_string_view(' ') | rv::filter_empty | rv::get1 | rv::to_int;

            a.push_back(v1);
            b.push_back(v2);
        }

        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end());

        for (int i = 0; i < a.size(); i++) {
            auto count = std::count_if(b.begin(), b.end(), [a = a[i]](int b) { return b == a; });
            result += count * a[i];;
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
    process("sample.txt");
    process("input.txt");
    return 0;
}
