#include "../../common_fast.h"

using Ints = Vector<int>;
using BitSet = std::bitset<128>;

auto compute_group = [](auto row, const BitSet values, const int vlen, int &pos) {
    int current = 0;
    int v = 0;
    pos = 0;

    for (auto c : row) {
        if (c == '?') {
            if (v >= vlen) {
                return -1;
            }
            c = values[v++] ? '#' : '.';
        }
        switch (c) {
            case '#':
                current++;
                break;
            case '.':
                if (current) {
                    return current;
                }
        }
        pos++;
    }

    return current;
};

void match(StringView row, const int group, BitSet values, const int vlen, Map<int, int> &counts) {
    int pos;
    auto test_group = compute_group(row, values, vlen, pos);

    if (test_group != -1) {

        if (test_group == group) {
            counts[pos]++;
        }

        return;
    }

    auto nv = values;
    nv.set(vlen, true);
    match(row, group, nv, vlen + 1, counts);
    nv.set(vlen, false);
    match(row, group, nv, vlen + 1, counts);
};

void doit2(StringView row, int offset, const Ints &groups, int group_index, int count, int64_t &final_count) {

    while (row[offset] == '.') {
        offset++;
    }

    if (offset >= row.size())
        return;

    /* for (int n = 1; offset + n <= row.size(); n++) { */

    auto str = row.substr(offset);
    /* log << str << endl; */
    int64_t new_count = 0;

    Map<int, int> counts;

    match(str, groups[group_index], {}, 0, counts);

    for (auto &kv : counts) {
        /* log << kv.first << " " << kv.second << "\n"; */
        auto ncount = count * kv.second;
        if (group_index < groups.size() - 1) {
            doit2(row, offset + kv.first + 1, groups, group_index + 1, ncount, final_count);
        } else {
            /* log << "yep!" << count << endl; */
            final_count += ncount;
        }
    }
    /* log << str << " | " << groups[group_index] << " = " << new_count << endl; */

    /* if (new_count) { */
    /*     count *= new_count; */
    /*     if (group_index < groups.size() - 1) { */
    /*         doit2(row, offset + n + 1, groups, group_index + 1, count, final_count); */
    /*     } else { */
    /*         /1* log << "yep!" << count << endl; *1/ */
    /*         final_count += count; */
    /*     } */
    /* } */
    /* } */
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
            doit2(row, 0, groups, 0, 1, count);

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
            doit2(final_row, 0, final_groups, 0, 1, count);

            return count;
        };

        for (auto line : lines) {

            log << "doing " << line << endl;

            auto r = compute_arrangements(line);

            log << " : " << r << endl;

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
