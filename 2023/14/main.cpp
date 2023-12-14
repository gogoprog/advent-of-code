#include "../../common_fast.h"

struct Context {

    Vector<Coord> rocks;
    StringViews lines;
    int width, height;
    Map<Coord, bool> rockMap;

    bool isValid(Coord c) const {
        return c.x >= 0 && c.x < width && c.y >= 0 && c.y < height;
    }

    char getChar(Coord c) const {
        return lines[c.y][c.x];
    }

    bool isFree(Coord c) const {
        auto it = rockMap.find(c);
        return (it == rockMap.end() || !it->second) && getChar(c) != '#';
    }

    void draw() {
        log << "draw:\n";
        for (int y{0}; y < height; ++y) {
            for (int x{0}; x < width; ++x) {
                Coord pos{x, y};
                auto ch = lines[y][x];

                if (ch == '#')
                    log << ch;
                else if (rockMap[pos]) {
                    log << 'O';
                } else {
                    log << '.';
                }
            }

            log << endl;
        }
    }

    void part1(auto _lines) {
        auto result{0};

        rs::copy(_lines, std::back_inserter(lines));

        width = lines[0].size();
        height = lines.size();

        for (int y{0}; y < height; ++y) {
            for (int x{0}; x < width; ++x) {
                Coord pos{x, y};
                auto ch = lines[y][x];

                if (ch == 'O') {
                    rockMap[pos] = true;
                    rocks.push_back(pos);
                }
            }
        }

        auto moving = false;

        do {
            moving = false;

            for (auto &rock : rocks) {
                Coord nc = rock;

                nc.y -= 1;

                if (isValid(nc) && isFree(nc)) {
                    rockMap[rock] = false;
                    rockMap[nc] = true;
                    rock = nc;
                    moving = true;
                }
            }

        } while (moving);

        for (auto &kv : rockMap) {
            if (kv.second) {
                result += (height - kv.first.y);
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0};

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
