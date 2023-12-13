#include "../../common_fast.h"

using Ints = Vector<int>;
using BitSet = std::bitset<128>;

int64_t factorial(int64_t n) {
    if (n == 0 || n == 1)
        return 1;
    return n * factorial(n - 1);
}

auto compute_groups = [](auto row, Ints &groups) {
    int current = 0;

    groups.resize(0);

    auto check = [&]() {
        if (current) {
            groups.push_back(current);
            current = 0;
        }
    };

    for (auto c : row) {
        switch (c) {
            case '#':
                current++;
                break;
            case '.':
                check();
                break;
        }
    }

    check();
};
auto compute_groups2 = [](auto row, Ints &groups, const BitSet values, const int vlen) {
    int current = 0;
    int v = 0;

    groups.resize(0);

    auto check = [&]() {
        if (current) {
            groups.push_back(current);
            current = 0;
        }
    };

    for (auto c : row) {
        if (c == '?') {
            if (v >= vlen) {
                return false;
            }
            c = values[v++] ? '#' : '.';
        }
        switch (c) {
            case '#':
                current++;
                break;
            case '.':
                check();
                break;
        }
    }

    check();

    return true;
};

void doit(StringView row, const Ints &groups, BitSet values, const int vlen, int64_t &count) {

    Ints test_groups;
    auto full = compute_groups2(row, test_groups, values, vlen);

    if (full) {

        if (test_groups == groups) {
            count++;
            return;
        }

        return;
    }

    bool cancel = false;

    if (test_groups.size() < groups.size()) {

        if (test_groups.size() > 1) {

            for (int i = 0; i < test_groups.size() - 1; i++) {
                if (test_groups[i] != groups[i]) {
                    cancel = true;
                    break;
                }
            }
        }

        if (!cancel) {
            auto nv = values;
            nv.set(vlen, true);
            doit(row, groups, nv, vlen + 1, count);
            nv.set(vlen, false);
            doit(row, groups, nv, vlen + 1, count);
        }
    } else {
        auto nv = values;
        nv.set(vlen, false);
        doit(row, groups, nv, vlen + 1, count);
    }
};

struct Context {

    void part1(auto lines) {
        auto result{0};

        auto compute_arrangements = [&](auto line) {
            auto row = line | rv::split_sv(' ') | rv::get0;
            auto groups_v = line | rv::split_sv(' ') | rv::get1 | rv::split_sv(',') | rv::to_ints;

            Vector<int> groups;
            rs::copy(groups_v, std::back_inserter(groups));

            int64_t count = 0;
            doit(row, groups, {}, 0, count);

            return count;
        };

        for (auto line : lines) {

            auto r = compute_arrangements(line);

            log << line << " : " << r << endl;

            result += r;
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        int64_t result{0};

        auto compute_arrangements = [&](auto line) {
            auto row = line | rv::split_sv(' ') | rv::get0;
            auto groups_v = line | rv::split_sv(' ') | rv::get1 | rv::split_sv(',') | rv::to_ints;

            Vector<int> groups;
            rs::copy(groups_v, std::back_inserter(groups));

            String final_row;
            Ints final_groups;
            bool first = true;

            for (int i = 0; i < 5; ++i) {
                if (!first) {
                    final_row += '?';
                }

                final_row += row;

                first = false;

                final_groups.insert(final_groups.end(), groups.begin(), groups.end());
            }

            int64_t count = 0;
            doit(final_row, final_groups, {}, 0, count);

            return count;
        };

        for (auto line : lines) {
            auto r = compute_arrangements(line);

            log << line << " : " << r << endl;

            result += r;
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
    /* process("input.txt"); */
    return 0;
}
