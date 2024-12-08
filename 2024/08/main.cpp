#include "../../common_fast.h"

struct Context {

    Map<char, Vector<Coord>> antennas;
    StringViews lines;
    int width, height;

    bool isValid(Coord coord) const {
        return coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height;
    }

    inline char getChar(Coord coord) const {
        return lines[coord.y][coord.x];
    }

    void parse(auto _lines) {
        lines = {};
        rs::copy(_lines, std::back_inserter(lines));
        width = lines[0].size();
        height = lines.size();

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Coord coord{x, y};
                char c = lines[y][x];

                if (c != '.') {
                    antennas[c].push_back(coord);
                }
            }
        }
    }

    void part1(auto lines) {
        auto result{0_int64};

        Set<Coord> antinodes;

        for (const auto &kv : antennas) {
            auto &key = kv.first;
            auto &values = kv.second;

            for (int i = 0; i < values.size(); ++i) {
                for (int j = i + 1; j < values.size(); ++j) {
                    if (i != j) {
                        auto a = values[i];
                        auto b = values[j];

                        auto delta = b - a;

                        auto na = a - delta;
                        auto nb = b + delta;

                        if (isValid(na)) {
                            antinodes.insert(na);
                        }
                        if (isValid(nb)) {
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

        for (const auto &kv : antennas) {
            auto &key = kv.first;
            auto &values = kv.second;

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

                        while (isValid(na)) {
                            antinodes.insert(na);
                            na = na - delta;
                        }
                        while (isValid(nb)) {
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
