#include "../../common.h"

struct System {
    Map<Point, bool> tiles;

    static Point get(const Point from, const String direction) {

        if (direction == "e")
            return {from.x + 1, from.y};
        if (direction == "se")
            return {from.x, from.y - 1};
        if (direction == "ne")
            return {from.x + 1, from.y + 1};
        if (direction == "sw")
            return {from.x - 1, from.y - 1};
        if (direction == "nw")
            return {from.x, from.y + 1};
        if (direction == "w")
            return {from.x - 1, from.y};

        throw "nop";
        return from;
    }

    int count() {
        int r{0};
        for (auto &kv : tiles) {

            if (kv.second) {
                r++;
            }
        }

        return r;
    }

    void process(const String line) {
        int i{0};
        Point point{0, 0};

        while (i < line.length()) {
            char c = line[i];

            if (c == 's' || c == 'n') {
                point = get(point, line.substr(i, 2));
                i += 2;
            } else {
                point = get(point, line.substr(i, 1));

                i++;
            }
        }
        tiles[point] = !tiles[point];
    }

    void update() {

        auto [min, max] = getMinMax(tiles);

        Map<Point, bool> newTiles;
        auto copy = tiles;

        for (int x{min.x - 1}; x <= max.x + 1; ++x) {
            for (int y{min.y - 1}; y <= max.y + 1; ++y) {

                Point point{x, y};

                auto value = tiles[point];
                int n{0};

                if (tiles[get(point, "ne")])
                    n++;
                if (tiles[get(point, "nw")])
                    n++;
                if (tiles[get(point, "sw")])
                    n++;
                if (tiles[get(point, "se")])
                    n++;
                if (tiles[get(point, "e")])
                    n++;
                if (tiles[get(point, "w")])
                    n++;

                if (value) {
                    if (n == 0 || n > 2) {
                    } else {
                        newTiles[point] = true;
                    }
                } else {
                    if (n == 2) {
                        newTiles[point] = true;
                    }
                }
            }
        }

        tiles = newTiles;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    System system{};

    for (auto &line : lines) {
        system.process(line);
    }

    log << "Part1: " << system.count() << endl;

    for (int i{0}; i < 100; i++) {
        system.update();
    }

    log << "Part2: " << system.count() << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
