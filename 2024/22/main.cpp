#include "../../common_fast.h"

struct Context {

    Vector<int> inputs;

    /* struct Result { */
    /*     Vector<int> changes; */
    /*     int price; */
    /* }; */

    Map<int, Map<Vector<int>, int>> results;

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

    Uint64 evolveTo(Uint64 input, int count) {
        for (int i = 0; i < count; ++i) {
            input = evolve(input, i);
        }

        /* log << input << endl; */

        return input;
    }

    Uint64 evolve2(Uint64 input, int count) {
        List<int> changes;

        auto &result = results[input];

        for (int i = 0; i < count; ++i) {
            auto next = evolve(input, i);

            changes.push_back(int((next % 10 - input % 10)));

            if (changes.size() > 4) {
                changes.pop_front();

                Vector<int> new_changes(changes.begin(), changes.end());

                if (result.find(new_changes) == result.end()) {
                    result[new_changes] = int(next % 10);
                }
            }

            input = next;
        }

        return input;
    }

    int compute(const Vector<int> &changes) {
        int result{0};

        for (auto &kv : results) {
            auto it = kv.second.find(changes);

            if (it != kv.second.end()) {
                result += it->second;
            }
        }

        return result;
    }

    int computeBest() {

        int best = 0;

        Set<Vector<int>> visited;

        for (auto &kv : results) {
            for (auto &kv2 : kv.second) {

                if (!visited.contains(kv2.first)) {
                    int r = compute(kv2.first);

                    visited.insert(kv2.first);
                    if (r > best) {
                        best = r;
                    }
                }
            }
        }

        return best;
    }

    void parse(auto lines) {
        rs::copy(lines | rv::to_ints, std::back_inserter(inputs));
    }

    void part1() {
        auto result{0_int64};

        for (auto input : inputs) {
            result += evolveTo(input, 2000);
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0_int64};

        for (auto input : inputs) {
            evolve2(input, 2000);
        }

        result = computeBest();

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
    process("sample2.txt");
    process("input.txt");
    return 0;
}
