#include "../../common_fast.h"

struct Entry {
    union {
        Coord buttons[2];
        struct {
            Coord a;
            Coord b;
        };
    };
    Point<Int64> prize;
};

struct Context {

    Vector<Entry> entries;

    void parse(auto blocks) {

        for (auto block : blocks) {
            entries.push_back({});
            auto &entry = entries.back();

            auto lines = block | rv::split_sv('\n');

            auto line_a = lines | rv::get0;
            auto line_b = lines | rv::get1;
            auto line_p = lines | rv::get2;

            entry.a.x = line_a | rv::split_sv('+') | rv::get1 | rv::to_int;
            entry.a.y = line_a | rv::split_sv('+') | rv::get2 | rv::to_int;

            entry.b.x = line_b | rv::split_sv('+') | rv::get1 | rv::to_int;
            entry.b.y = line_b | rv::split_sv('+') | rv::get2 | rv::to_int;

            entry.prize.x = line_p | rv::split_sv('=') | rv::get1 | rv::to_int;
            entry.prize.y = line_p | rv::split_sv('=') | rv::get2 | rv::to_int;
        }
    }

    void part1() {
        auto result{0_int64};

        for (auto &entry : entries) {

            auto a = entry.a;
            auto b = entry.b;
            auto p = entry.prize;

            double j = (p.y * a.x - p.x * a.y) / double(b.y * a.x - b.x * a.y);
            double i = (p.x - j * b.x) / double(a.x);

            if (i == int(i) && j == int(j)) {
                result += i * 3 + j;
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0_int64};

        for (auto entry : entries) {
            entry.prize.x += 10000000000000;
            entry.prize.y += 10000000000000;

            auto a = entry.a;
            auto b = entry.b;
            auto p = entry.prize;

            double j = (p.y * a.x - p.x * a.y) / double(b.y * a.x - b.x * a.y);
            double i = (p.x - j * b.x) / double(a.x);

            if (i == Int64(i) && j == Int64(j)) {
                result += i * 3 + j;
            }
        }

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto blocks = content | rv::split_sv("\n\n");
    {
        Context context;
        context.parse(blocks);
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
