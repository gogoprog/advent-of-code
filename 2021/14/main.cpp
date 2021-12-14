#include "../../common.h"

struct Context {
    String initial;
    Map<String, char> rules;

    Map<String, ull> pairCount;
    Map<String, ull> nextPairCount;

    void addRule(const String &rule) {
        InputStringStream iss{rule};
        String pair, temp;
        char c;
        iss >> pair >> temp >> c;

        rules[pair] = c;
    }

    void init() {
        for (int i = 0; i < initial.size() - 1; ++i) {
            pairCount[{initial[i], initial[i + 1]}] += 1;
        }
    }

    void step() {
        nextPairCount.clear();

        for (auto &kv : pairCount) {
            auto &key = kv.first;
            auto value = kv.second;

            nextPairCount[{key[0], rules[key]}] += value;
            nextPairCount[{rules[key], key[1]}] += value;
        }

        std::swap(nextPairCount, pairCount);
    }

    ull compute() {
        Map<char, ull> counts;

        for (auto &kv : pairCount) {
            char ch = kv.first[0];
            counts[ch] += kv.second;
        }

        counts[initial.back()]++;

        auto [min, max] =
            std::minmax_element(counts.begin(), counts.end(), [](auto &a, auto &b) { return a.second < b.second; });

        return max->second - min->second;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx{lines[0]};

    for (auto &line : lines | rv::drop(2)) {
        ctx.addRule(line);
    }

    ctx.init();

    for (auto i : rs::iota_view(0, 10)) {
        ctx.step();
    }

    log << "Part1: " << ctx.compute() << endl;

    for (auto i : rs::iota_view(0, 30)) {
        ctx.step();
    }

    log << "Part2: " << ctx.compute() << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
