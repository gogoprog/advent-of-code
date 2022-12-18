#include "../../common.h"

enum Shape { Horizontal, Plus, FlipL, Vertical, Square, Count };

Map<int, Vector<Coord>> shapeDefs = {
    {Horizontal, {{0, 0}, {1, 0}, {2, 0}, {3, 0}}},       //
    {Plus, {{1, 0}, {0, -1}, {1, -1}, {2, -1}, {1, -2}}}, //
    {FlipL, {{0, 0}, {1, 0}, {2, 0}, {2, -1}, {2, -2}}},  //
    {Vertical, {{0, 0}, {0, -1}, {0, -2}, {0, -3}}},      //
    {Square, {{0, 0}, {1, 0}, {1, -1}, {0, -1}}},         //
};

struct Block {
    Coord position;
    Shape shape;
};

struct Context {
    int width{7};
    String jetPattern;
    Map<Coord, bool> rocks;
    int highest{1};
    int currentShape = Horizontal;
    int currentJet = 0;

    void parse(const Strings &lines) {
        jetPattern = lines[0];
    }

    bool isRock(const Coord pos) {
        return rocks[pos];
    }

    bool isValid(const Coord pos, const int shape) {
        auto &def = shapeDefs[shape];

        for (auto &pos2 : def) {
            auto npos = pos + pos2;
            if (isRock(npos)) {
                return false;
            }

            if (npos.y == 1)
                return false;
            if (npos.x < 0)
                return false;
            if (npos.x >= width)
                return false;
        }

        return true;
    }

    void markRocks(const Coord pos, const int shape) {
        auto &def = shapeDefs[shape];

        for (auto &pos2 : def) {
            auto npos = pos + pos2;
            rocks[npos] = true;

            highest = std::min(highest, npos.y);
        }
    }

    void draw() {

        log << "=" << endl;

        for (int i = highest; i <= 0; i++) {
            log << "|";
            for (int x = 0; x < width; ++x) {
                if (isRock({x, i})) {
                    log << '#';
                } else {
                    log << '.';
                }
            }
            log << "|";
            log << endl;
        }

        log << "+-------+" << endl;
    }

    void process() {
        Coord pos{2, highest - 4};
        Coord new_pos;

        while (true) {
            new_pos = pos;
            new_pos.x += jetPattern[currentJet] == '>' ? 1 : -1;
            currentJet = (currentJet + 1) % jetPattern.size();

            if (!isValid(new_pos, currentShape)) {
                new_pos.x = pos.x;
            }

            new_pos.y++;

            if (!isValid(new_pos, currentShape)) {
                new_pos.y = pos.y;
                markRocks(new_pos, currentShape);
                break;
            }
            pos = new_pos;
        }
        currentShape = (currentShape + 1) % Count;
    }

    void part1() {
        int i = 0;

        while (i < 2022) {
            process();
            ++i;
        }

        auto result{1 - highest};
        log << "Part1: " << result << endl;
    }

    void part2() {
        int i = 0;

        struct History {
            Array<bool, 7> values;
            int shape;
            int jet;
            int highest;

            bool operator==(const History &other) const {
                return jet == other.jet && shape == other.shape && values == other.values;
            }
        };

        Vector<History> history;

        auto is_same = [&](const int index, const int index2) { return history[index] == history[index2]; };

        while (true) {
            history.push_back({});
            auto &last = history.back();
            last.shape = currentShape;
            last.jet = currentJet;

            process();

            auto &values = last.values;
            for (auto x : rs::iota_view(0, 7)) {
                values[x] = rocks[{x, highest}];
            }

            last.highest = highest;

            bool found{false};

            for (int j{3}; j < i; j++) {

                bool same = true;

                for (int k = 0; k < 8; k++) {
                    if (!is_same(j - k, i - k)) {
                        same = false;
                    }
                }

                if (same) {
                    found = true;

                    int first = j;
                    int interval = i - j;
                    int h0 = 1 - history[first - 1].highest;
                    int h1 = 1 - history[first].highest;
                    int delta = (1 - highest) - h1;

                    ull target = 1000000000000;

                    ull rest = (target - first) % interval;
                    ull times = (target - first) / interval;

                    ull h = (1 - history[first - 1 + rest].highest) - h0;

                    ull result = h0 + times * delta + h;

                    log << "Part2: " << result << endl;

                    break;
                }
            }
            if (found)
                break;

            ++i;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        Context context;
        context.parse(lines);
        context.part1();
    }
    {
        Context context;
        context.parse(lines);
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
