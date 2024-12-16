#include "../../common_fast.h"

static constexpr Array<Coord, 4> deltas = {Coord{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

struct Context {

    Grid grid;

    void parse(auto lines) {
        grid.parse(lines);
    }

    template <int part> int process(auto start) {
        int result{0};
        Set<Coord> seen;

        Queue<Coord> q;

        q.push(start);

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            if (grid.get(node) == '9') {
                if constexpr (part == 1) {
                    seen.insert(node);
                } else {
                    result++;
                }
            } else {

                for (auto &delta : deltas) {
                    auto npos = node + delta;

                    if (grid.isValid(npos)) {
                        if (grid.get(npos) - grid.get(node) == 1) {
                            q.push(npos);
                        }
                    }
                }
            }
        }

        if constexpr (part == 1) {
            result = seen.size();
        }

        return result;
    }

    void part1(auto lines) {
        auto result{0_int64};

        grid.for_each([&](auto coord, auto c) {
            if (c == '0') {
                result += process<1>(coord);
            }

            return true;
        });

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};

        grid.for_each([&](auto coord, auto c) {
            if (c == '0') {
                result += process<2>(coord);
            }
            return true;
        });

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
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
