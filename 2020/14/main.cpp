#include "../../common.h"

struct Program {
    Vector<ull> masks{0, 0};
    ull xmask;
    Map<ull, ull> mems;
    Map<ull, ull> mems2;

    void setMask(const String line) {
        masks[0] = 0;
        masks[1] = 0;

        auto len = line.length();

        for (ull i = 0; i < len; i++) {
            ull j = len - i - 1;
            char c = line[j];

            if (c != 'X') {
                auto &mask = masks[c - '0'];
                mask |= (1ULL << i);
            }
        }

        xmask = ~(masks[0] | masks[1]);
    }

    void write(const ull mem, ull value) {
        value |= masks[1];
        value &= ~masks[0];
        mems[mem] = value;
    }

    void write2(ull mem, const ull value) {
        mem |= masks[1];

        writeXMask(mem, value, 0);
    }

    void writeXMask(const ull mem, const ull value, const ull from) {
        for (ull i = from; i < 36; i++) {
            auto v = (1ULL << i);

            if (xmask & v) {
                mems2[mem | v] = value;
                mems2[mem & (~v)] = value;
                writeXMask(mem | v, value, i + 1);
                writeXMask(mem & (~v), value, i + 1);
            }
        }
    }

    template <bool part2 = true> void parse(const String line) {
        InputStringStream iss{line};
        char c;

        if (line[1] == 'a') {
            String word, line;
            iss >> line >> c >> line;
            setMask(line);
        } else {
            ull a, b;
            iss >> c >> c >> c >> c >> a >> c >> c >> b;

            write(a, b);

            if constexpr (part2) {
                write2(a, b);
            }
        }
    }

    void part1() {
        ull r{0};

        for (auto &kv : mems) {
            r += kv.second;
        }

        log << "Part1: " << r << endl;
    }

    void part2() {
        ull r{0};

        for (auto &kv : mems2) {
            r += kv.second;
        }

        log << "Part2: " << r << endl;
    }
};

template <bool part2 = true> void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Program program;

    for (const auto &line : lines) {
        program.parse<part2>(line);
    }

    program.part1();

    if constexpr (part2) {
        program.part2();
    }
}

int main() {
    process<false>("sample.txt");
    process("sample2.txt");
    process("input.txt");
    return 0;
}
