#include "../../common.h"

struct Context {
    Map<Coord, bool> rocks;
    Map<Coord, bool> sands;

    int bottom{0};

    void parse(const Vector<String> &input) {
        for (auto str : input) {
            InputStringStream iss{str};
            String tmp;

            Vector<Coord> coords;

            while (!iss.eof()) {
                iss >> tmp;
                char c;
                Coord pos;
                InputStringStream iss2{tmp};
                iss2 >> pos.x >> c >> pos.y;
                rocks[pos] = true;
                coords.push_back(pos);
                iss >> tmp;
            }

            for (auto i = 1; i < coords.size(); i++) {
                auto from = coords[i - 1];
                auto to = coords[i];
                auto delta = to - from;
                auto size = delta.manhattan();
                auto dir = delta / size;

                auto current = from;

                rocks[current] = true;
                bottom = std::max(current.y, bottom);

                while (true) {
                    current += dir;

                    rocks[current] = true;
                    bottom = std::max(current.y, bottom);

                    if (current == to) {
                        break;
                    }
                }
            }
        }
    }

    bool isRock(const Coord pos) {
        return rocks[pos];
    }

    bool isBlocked(const Coord pos) {
        return rocks[pos] || sands[pos];
    }

    template <bool part2 = false> bool produceSand() {
        Coord pos{500, 0};
        auto start = pos;

        while (true) {

            if (part2 && pos.y + 1 == bottom + 2) {
                break;
            } else if (isBlocked({pos.x, pos.y + 1})) {

                if (!isBlocked({pos.x - 1, pos.y + 1})) {
                    pos.x--;
                    pos.y++;
                } else if (!isBlocked({pos.x + 1, pos.y + 1})) {
                    pos.x++;
                    pos.y++;
                } else {
                    break;
                }
            } else {
                pos.y += 1;
            }

            if (!part2 && pos.y == bottom) {
                return true;
            }
        }

        sands[pos] = true;

        if (part2 && pos == start) {
            return true;
        }

        return false;
    }

    void draw() {
        log << "draw:" << endl;

        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                Coord pos = {x + 495, y};

                if (rocks[pos]) {
                    log << '#';
                } else if (sands[pos]) {
                    log << 'o';
                } else {
                    log << '.';
                }
            }
            log << endl;
        }
    }

    void part1() {
        auto i = 0;
        while (!produceSand()) {
            ++i;
        }

        log << "Part1: " << i << endl;
    }

    void reset() {
        sands = {};
    }

    void part2() {
        auto i = 0;
        while (!produceSand<true>()) {
            ++i;
        }

        i++;

        log << "Part2: " << i << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx;
    ctx.parse(lines);

    ctx.part1();
    ctx.reset();
    ctx.part2();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
