#include "../../common_fast.h"

using Ints = Vector<int>;

bool can_match(StringView row, const int group) {

    int i = 0;
    int current = 0;

    while (row[i] != '.') {
        current++;

        if (current == group) {

            return row[i + 1] != '#';
        }

        ++i;
    }

    return false;
}

void doit2(StringView row, int offset, const Ints &groups, int group_index, int count, int64_t &final_count) {

    /* static Map<int, Map<String, CacheEntry>> cache; */

    if (offset >= row.size()) {
        return;
    }

    auto group = *(groups.begin() + group_index);

    for (int i = offset; i < row.size(); i++) {

        if (row[i] == '.') {
            continue;
        }

        if (row[i - 1] != '#') {

            auto str = row.substr(i, group + 1);

            auto match = can_match(str, group);
            /* log << str << " match " << group << "(" << group_index << ") = " << match << endl; */

            if (match) {

                if (group_index == groups.size() - 1) {

                    auto rest = row.substr(i + group + 1);

                    if (rest.find('#') == StringView::npos) {
                        ++final_count;
                    }
                } else {
                    doit2(row, i + group + 1, groups, group_index + 1, count, final_count);
                }
            }
            if (row[i] == '#')
                break;
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
            String final_row = String(row);

            final_row = '.' + final_row + '.';
            doit2(final_row, 0, groups, 0, 1, count);

            return count;
        };

        for (auto line : lines) {

            auto r = compute_arrangements(line);

            /* log << line << " : " << r << endl; */

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

            final_row = '.' + final_row + '.';

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
    process("sample2.txt");
    /* process("sample.txt"); */
    /* process("input.txt"); */
    return 0;
}
