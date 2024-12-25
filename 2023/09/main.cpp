#include "../../common_fast.h"

struct Context {

    void fillRows(auto line, Vector<Vector<int64_t>> &rows) {
        auto ints = line | rv::split_sv(' ') | rv::to_ints;
        auto &row = rows.emplace_back();
        rs::copy(ints.begin(), ints.end(), std::back_inserter(row));
        auto index = 0;
        bool all_zero = false;

        do {
            auto current_row = rows[index];
            auto deltas =
                current_row | rv::adjacent<2> | rv::transform([](auto pair) { return std::get<1>(pair) - std::get<0>(pair); });
            auto &new_row = rows.emplace_back();
            rs::copy(deltas.begin(), deltas.end(), std::back_inserter(new_row));

            ++index;

            all_zero = std::all_of(new_row.begin(), new_row.end(), [](auto delta) { return delta == 0; });
        } while (!all_zero);
    }

    void part1(auto lines) {
        auto result{0};

        for (auto line : lines) {

            Vector<Vector<int64_t>> rows;

            fillRows(line, rows);

            rows.back().push_back(0);

            for (int64_t i = rows.size() - 2; i >= 0; --i) {
                auto &row = rows[i];
                auto lastindex = row.size() - 1;
                auto value = rows[i + 1][lastindex] + row[lastindex];

                row.push_back(value);
            }

            result += rows[0].back();
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0};

        for (auto line : lines) {
            Vector<Vector<int64_t>> rows;

            fillRows(line, rows);

            rows.back().insert(rows.back().begin(), 0);

            for (int64_t i = rows.size() - 2; i >= 0; --i) {
                auto &row = rows[i];
                auto value = -rows[i + 1][0] + row[0];

                row.insert(row.begin(), value);
            }

            result += rows[0].front();
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
