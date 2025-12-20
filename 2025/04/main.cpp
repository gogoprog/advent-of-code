#include "../../common_fast.h"

static const Vector<Coord> &deltas = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

struct Context {

    Grid grid;

    void parse(auto lines) {
        grid.parse(lines);
    }

    void part1(auto lines) {
        auto result{0_int64};

        grid.for_each([&](auto pos, auto value) {
            if (value == '@') {
                int count{0};
                for (auto delta : deltas) {
                    auto newpos = pos + delta;
                    if (grid.isValid(newpos)) {
                        if (grid.get(newpos) == '@') {
                            count++;
                        }
                    }
                }

                if (count < 4) {
                    result++;
                }
            }
        });

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};

        bool work{true};

        Set<Coord> removed;

        while (work) {
            work = false;

            grid.for_each([&](auto pos, auto value) {
                if (value == '@') {
                    if (removed.find(pos) == removed.end()) {
                        int count{0};
                        for (auto delta : deltas) {
                            auto newpos = pos + delta;
                            if (grid.isValid(newpos)) {
                                if (grid.get(newpos) == '@') {
                                    if (removed.find(newpos) == removed.end()) {
                                        count++;
                                    }
                                }
                            }
                        }

                        if (count < 4) {
                            work = true;
                            removed.insert(pos);
                        }
                    }
                }
            });
        }

        result = removed.size();

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
