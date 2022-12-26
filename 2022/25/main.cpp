#include "../../common.h"

Map<char, int> values = {
    {'2', 2}, {'1', 1}, {'0', 0}, {'-', -1}, {'=', -2},
};

Map<int, char> representations = {
    {2, '2'}, {1, '1'}, {0, '0'}, {-1, '-'}, {-2, '='},
};

using Snafu = Map<int, int>;

lli toDecimal(const Snafu input) {
    lli result = 0;

    for (auto kv : input) {
        result += pow(5, kv.first) * kv.second;
    }
    return result;
}

Snafu fromString(const String input) {
    Snafu result;
    for (int c = 0; c < input.size(); c++) {
        auto rank = input.size() - 1 - c;
        auto value = values[input[c]];

        result[rank] = value;
    }

    return result;
}

String toString(Snafu input) {
    String str;

    bool first = true;
    for (auto rank = 100; rank >= 0; --rank) {
        auto v = input[rank];

        if (first && !v)
            continue;

        str += representations[v];

        first = false;
    }

    return str;
}

Snafu sum(Snafu a, Snafu b) {
    Snafu result;

    Map<int, int> reports;

    for (auto rank = 0; rank < 100; ++rank) {
        auto va = a[rank];
        auto vb = b[rank];
        auto v = va + vb;

        result[rank] = v;
    }

    bool simplify = true;

    while (simplify) {
        simplify = false;

        for (auto &kv : result) {
            auto rank = kv.first;
            auto v = kv.second;

            if (v < -2) {
                simplify = true;
                result[rank + 1] -= 1;
                result[rank] += 5;
            } else if (v > 2) {
                simplify = true;
                result[rank + 1] += 1;
                result[rank] -= 5;
            }
        }
    }

    return result;
}

struct Context {

    Strings lines;

    void parse(const Strings &lines) {
        this->lines = lines;
    }

    void part1() {
        lli result{0};
        Snafu snresult;

        for (auto line : lines) {
            auto snafu = fromString(line);
            auto tostr = toString(snafu);
            myassert(line, tostr);

            auto deci = toDecimal(snafu);
            snresult = sum(snresult, snafu);

            result += deci;

            auto deci2 = toDecimal(snresult);
            myassert(result, deci2);
        }

        log << "Part1: " << toString(snresult) << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        Context context;
        context.parse(lines);
        context.part1();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
