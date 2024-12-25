#include "../../common_fast.h"

struct Context {
    static constexpr Array<Coord, 8> deltas = {Coord{0, -1}, {0, 1},  {-1, 0}, {1, 0},
                                               {1, 1},       {1, -1}, {-1, 1}, {-1, -1}};

    static constexpr Array<Coord, 4> corners = {Coord{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};

    static constexpr String target = "XMAS";

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
    }

    void part1(auto lines) {
        auto result{0};
        parse(lines);

        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                Coord start_pos = {x, y};

                for (auto delta : deltas) {
                    int i = 0;
                    auto pos = start_pos;

                    while (isValid(pos)) {
                        char c = getChar(pos);

                        if (c != target[i]) {
                            break;
                        } else {
                            pos += delta;
                            i++;

                            if (i == 4) {
                                result++;
                                break;
                            }
                        }
                    }
                }
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0};

        parse(lines);

        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                const Coord pos = {x, y};

                if (getChar(pos) == 'A') {
                    char values[4];
                    bool valid = true;

                    for (int i = 0; i < 4; ++i) {
                        auto delta = corners[i];
                        auto npos = pos + delta;

                        char c = getChar(npos);

                        if (c != 'M' && c != 'S') {
                            valid = false;
                            break;
                        } else {
                            values[i] = c;
                        }
                    }

                    if (valid) {
                        if (values[0] != values[2] && values[1] != values[3]) {
                            result++;
                        }
                    }
                }
            }
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
