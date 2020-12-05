#include "../../common.h"

Vector2 computeId(const String input) {

    Pair<int, int> rows{0, 127};
    Pair<int, int> cols{0, 7};

    for (char c : input) {

        switch (c) {
            case 'F': {
                rows.second = rows.first + (rows.second - rows.first) / 2;
            } break;
            case 'B': {
                rows.first = rows.second - (rows.second - rows.first) / 2;
            } break;
            case 'L': {
                cols.second = cols.first + (cols.second - cols.first) / 2;
            } break;
            case 'R': {
                cols.first = cols.second - (cols.second - cols.first) / 2;
            } break;
        }
    }

    return {cols.first, rows.first};
}

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    int best{0};

    Vector2 min{8, 128}, max{0, 0};
    Map<Point, bool> seats;

    for (auto &line : lines) {
        auto t = computeId(line);
        int id = t.y * 8 + t.x;
        if (id > best) {
            best = id;
        }

        min.x = std::min(min.x, t.x);
        min.y = std::min(min.y, t.y);
        max.x = std::max(max.x, t.x);
        max.y = std::max(max.y, t.y);

        seats[t] = true;
    }

    Vector2 min2{8, 128}, max2{0, 0};

    for (int x = min.x; x <= max.x; x++) {
        for (int y = min.y; y <= max.y; y++) {
            Point p{x, y};
            if (seats[p] == false) {
                min2.x = std::min(min2.x, p.x);
                min2.y = std::min(min2.y, p.y);
                max2.x = std::max(max2.x, p.x);
                max2.y = std::max(max2.y, p.y);
            }
        }
    }

    Vector2 finalSeat;
    for (int x = min.x; x <= max.x; x++) {
        for (int y = min.y; y <= max.y; y++) {
            Point p{x, y};
            if (seats[p] == false) {
                if (p.x > min2.x && p.x < max2.x && p.y > min2.y && p.y < max2.y) {
                    finalSeat = p;
                }
            }
        }
    }

    log << "part1: " << best << endl;
    log << "part2: " << finalSeat.y * 8 + finalSeat.x << endl;
}

int main() {
    /* process("samples.txt"); */
    process("input.txt");
    return 0;
}
