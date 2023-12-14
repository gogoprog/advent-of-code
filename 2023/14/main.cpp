#include "../../common_fast.h"

constexpr Array<Coord, 4> allMoves = {Coord{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

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

    void parse(auto _lines) {

        rs::copy(_lines, std::back_inserter(lines));

        width = lines[0].size();
        height = lines.size();
        rocks.resize(0);
        rockMap.clear();

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
    }

    void tilt(int dir) {
        auto moving = false;

        auto delta = allMoves[dir];

        do {
            moving = false;

            for (auto &rock : rocks) {
                Coord nc = rock + delta;

                if (isValid(nc) && isFree(nc)) {
                    rockMap[rock] = false;
                    rockMap[nc] = true;
                    rock = nc;
                    moving = true;
                }
            }

        } while (moving);
    }

    int compute() {
        auto result = 0;
        for (auto &rock : rocks) {

            result += (height - rock.y);
        }

        return result;
    }

    void part1(auto _lines) {
        parse(_lines);

        tilt(0);

        log << "Part1: " << compute() << endl;
    }

    void part2(auto lines) {
        auto result{0};

        auto cont = true;

        int i = 0;

        Vector<int> history;

        int current_repeat_start = -1;
        int current_repeat_count = 0;

        while (cont) {
            tilt(0);
            tilt(1);
            tilt(2);
            tilt(3);

            auto load = compute();

            if (current_repeat_start == -1) {

                auto it = std::find(history.begin(), history.end(), load);
                if (it != history.end()) {
                    current_repeat_start = it - history.begin();
                    current_repeat_count = 1;
                }
            } else {

                if (history[current_repeat_start + current_repeat_count] == load) {
                    current_repeat_count++;

                    if (current_repeat_count == 128) {

                        for (int i = 2; i < current_repeat_count / 4; ++i) {

                            auto good = true;
                            auto start = history[current_repeat_start];

                            for (int t = 0; t < 4; t++) {
                                for (int j = 0; j < i; j++) {
                                    if (history[current_repeat_start + i * t + j] !=
                                        history[current_repeat_start + j]) {
                                        good = false;
                                        break;
                                    }
                                }

                                if (!good)
                                    break;
                            }

                            if (good) {
                                result = history[current_repeat_start + ((1000000000 - 1 - current_repeat_start) % i)];
                                cont = false;
                                break;
                            }
                        }
                    }
                } else {
                    current_repeat_start = -1;
                    auto it = std::find(history.begin(), history.end(), load);
                    if (it != history.end()) {
                        current_repeat_start = it - history.begin();
                        current_repeat_count = 1;
                    }
                }
            }

            history.push_back(load);

            ++i;
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
