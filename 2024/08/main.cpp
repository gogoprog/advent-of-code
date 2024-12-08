#include "../../common_fast.h"

struct Context {
    Grid grid;

    Map<char, Vector<Coord>> antennas;

    void parse(auto lines) {
        grid.parse(lines);

        grid.for_each([&](auto coord, char c) {
            if (c != '.') {
                antennas[c].push_back(coord);
            }
        });
    }

    void part1(auto lines) {
        auto result{0_int64};

        Set<Coord> antinodes;

        for (const auto &[key, values] : antennas) {

            for (int i = 0; i < values.size(); ++i) {
                for (int j = i + 1; j < values.size(); ++j) {
                    if (i != j) {
                        auto a = values[i];
                        auto b = values[j];

                        auto delta = b - a;

                        auto na = a - delta;
                        auto nb = b + delta;

                        if (grid.isValid(na)) {
                            antinodes.insert(na);
                        }
                        if (grid.isValid(nb)) {
                            antinodes.insert(nb);
                        }
                    }
                }
            }
        }

        result = antinodes.size();

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};

        Set<Coord> antinodes;

        for (const auto &[key, values] : antennas) {

            for (int i = 0; i < values.size(); ++i) {
                for (int j = i + 1; j < values.size(); ++j) {
                    if (i != j) {
                        auto a = values[i];
                        auto b = values[j];

                        auto delta = b - a;

                        auto na = a - delta;
                        auto nb = b + delta;
                        antinodes.insert(a);
                        antinodes.insert(b);

                        while (grid.isValid(na)) {
                            antinodes.insert(na);
                            na = na - delta;
                        }
                        while (grid.isValid(nb)) {
                            antinodes.insert(nb);
                            nb += delta;
                        }
                    }
                }
            }
        }

        result = antinodes.size();

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
