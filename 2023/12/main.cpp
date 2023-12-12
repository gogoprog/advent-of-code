#include "../../common_fast.h"

using Ints = Vector<int>;

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
            case '?':
                check();
                return;

                break;
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

void doit(StringView row, const Ints &groups, String values, int64_t &count) {
    String str;
    int i = 0;
    bool complete = true;

    for (auto c : row) {

        switch (c) {
            case '?':
                if (i < values.length()) {
                    str += values[i];
                    i++;
                } else {
                    str += c;
                    complete = false;
                }

                break;
            case '#':
            case '.':
                str += c;
                break;
        }
    }

    if (complete) {
        Ints test_groups;
        compute_groups(str, test_groups);

        if (test_groups == groups) {
            count++;
        }
    } else {
        Ints test_groups;
        compute_groups(str, test_groups);

        bool cancel = false;

        if (test_groups.size() <= groups.size()) {

            if (test_groups.size() > 1) {

                for (int i = 0; i < test_groups.size() - 1; i++) {
                    if (test_groups[i] != groups[i]) {
                        cancel = true;
                        break;
                    }
                }
            }

            if (!cancel) {
                doit(row, groups, values + ".", count);
                doit(row, groups, values + "#", count);
            }
        }
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
            doit(row, groups, "", count);

            return count;
        };

        for (auto line : lines) {
            result += compute_arrangements(line);
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
            /* doit2(final_row, final_groups, "", count); */

            return count;
        };

        for (auto line : lines) {
            auto r = compute_arrangements(line);

            /* log << line << " : " << r << endl; */

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
    process("input.txt");
    return 0;
}
