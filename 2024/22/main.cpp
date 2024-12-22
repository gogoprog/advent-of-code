#include "../../common_fast.h"

struct Context {

    Vector<int> inputs;

    inline Uint64 mix(Uint64 input, const Uint64 secret) {
        return input ^ secret;
    }

    inline Uint64 prune(Uint64 input) {
        return input % 16777216;
    }

    Uint64 evolve(Uint64 secret, const int seq) {
        Uint64 result = secret * 64;

        result = mix(result, secret);
        result = prune(result);
        secret = result;

        result = result / 32;

        result = mix(result, secret);
        result = prune(result);
        secret = result;

        result = result * 2048;

        result = mix(result, secret);
        result = prune(result);

        return result;
    }

    Uint64 evolve2(Uint64 input, int count) {
        for (int i = 0; i < count; ++i) {
            input = evolve(input, i);
        }

        /* log << input << endl; */

        return input;
    }

    void parse(auto lines) {
        rs::copy(lines | rv::to_ints, std::back_inserter(inputs));
    }

    void part1() {
        auto result{0_int64};

        for (auto input : inputs) {
            result += evolve2(input, 2000);
        }

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
        context.parse(lines);
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
