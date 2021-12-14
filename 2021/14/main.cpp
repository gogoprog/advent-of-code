#include "../../common.h"

struct Context {
    String current;
    String next;
    Map<String, char> rules;

    void addRule(const String &rule) {
        InputStringStream iss{rule};
        String pair, temp;
        char c;
        iss >> pair >> temp >> c;

        rules[pair] = c;
    }

    void step() {

        next.resize(0);
        next += current[0];

        for (int i = 0; i < current.size() - 1; ++i) {

            StringView sv{current.begin() + i, current.begin() + i + 2};

            auto rule = std::find_if(rules.begin(), rules.end(), [&](auto &kv) { return kv.first == sv; });

            if (rule != rules.end()) {
                next += rule->second;
            }

            next += sv[1];
        }

        std::swap(current, next);
    }

    ull compute() {
        Set<char> chars;

        for (auto &kv : rules) {
            chars.insert(kv.first[0]);
            chars.insert(kv.first[1]);
            chars.insert(kv.second);
        }

        Map<char, ull> counts;

        for (auto ch : chars) {
            counts[ch] = std::count(current.begin(), current.end(), ch);
        }

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

    ull previous = 1;

    for (auto i : rs::iota_view(0, 20)) {
        ctx.step();
        auto now = ctx.compute();

        log << now / (double)previous << endl;
        previous = now;
    }

    /* ctx.compute(); */

    /* for (auto i : rs::iota_view(0, 30)) { */
    /*     ctx.step(); */
    /* } */

    /* ctx.compute(); */
}

int main() {
    process("sample.txt");
    /* process("input.txt"); */
    return 0;
}
