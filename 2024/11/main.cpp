#include "../../common_fast.h"

inline int countDigits(Int64 b) {
    Int64 n = 10;
    for (int i = 1; i < 16; ++i) {
        if (n > b) {
            return i;
        }

        n *= 10;
    }
    return 0;
}

inline Int64 pow10(int n) {
    Int64 result = 1;

    for (int i = 0; i < 16; ++i) {
        if (i >= n) {
            return result;
        }

        result *= 10;
    }

    return 0;
}

static Int64 countNodesAfter(Int64 n, const int iterations) {
    static Map<Pair<Int64, int>, Int64> cache;

    Int64 result;

    Pair<Int64, int> key{n, iterations};

    auto entry = cache.find(key);

    if (entry != cache.end()) {
        return entry->second;
    }

    if (iterations == 0) {
        return 1;
    }

    if (n == 0) {
        result = countNodesAfter(1, iterations - 1);
    } else if (int count = countDigits(n); (count & 1) == 0) {
        Int64 sc = pow10(count / 2);
        Int64 a = Int64(n / sc);
        Int64 b = n - (a * sc);
        auto totala = countNodesAfter(a, iterations - 1);
        auto totalb = countNodesAfter(b, iterations - 1);
        auto total = totala + totalb;
        result = total;
    } else {
        result = countNodesAfter(n * 2024, iterations - 1);
    }

    cache[key] = result;

    return result;
}

struct Context {

    List<Int64> initList;

    void parse(auto lines) {
        rs::copy(lines | rv::split_sv(' ') | rv::to_ints, std::back_inserter(initList));
    }

    void part1() {
        auto result{0_int64};

        for (auto n : initList) {
            result += countNodesAfter(n, 25);
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0_int64};

        for (auto n : initList) {
            result += countNodesAfter(n, 75);
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
