#include "../../common.h"

struct System {
    Map<Point, bool> tiles;

    Point min{0, 0}, max{0, 0};

    static Point get(const Point from, const char d, const char d2 = 0) {

        switch (d) {
            case 'w':
                return {from.x - 1, from.y};
            case 'e':
                return {from.x + 1, from.y};

            case 's':
                switch (d2) {
                    case 'e':
                        return {from.x, from.y - 1};
                    case 'w':
                        return {from.x - 1, from.y - 1};
                }

            case 'n':
                switch (d2) {
                    case 'e':
                        return {from.x + 1, from.y + 1};
                    case 'w':
                        return {from.x, from.y + 1};
                }
        }

        assert(false);

        return from;
    }

    int count() const {
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
                point = get(point, c, line[i + 1]);
                i += 2;
            } else {
                point = get(point, c);

                i++;
            }
        }
        tiles[point] = !tiles[point];

        setMinMax(point);
    }

    inline void setMinMax(const Point point) {
        min.y = std::min(min.y, point.y);
        max.y = std::max(max.y, point.y);
        min.x = std::min(min.x, point.x);
        max.x = std::max(max.x, point.x);
    }

    void update() {

        Map<Point, bool> newTiles;

        for (int x{min.x - 1}; x <= max.x + 1; ++x) {
            for (int y{min.y - 1}; y <= max.y + 1; ++y) {

                Point point{x, y};

                auto value = tiles[point];
                int n{0};

                if (tiles[get(point, 'n', 'e')])
                    n++;
                if (tiles[get(point, 'n', 'w')])
                    n++;
                if (tiles[get(point, 's', 'w')])
                    n++;
                if (tiles[get(point, 's', 'e')])
                    n++;
                if (tiles[get(point, 'e')])
                    n++;
                if (tiles[get(point, 'w')])
                    n++;

                if (value) {
                    if (n == 0 || n > 2) {
                    } else {
                        newTiles[point] = true;
                        setMinMax(point);
                    }
                } else {
                    if (n == 2) {
                        newTiles[point] = true;
                        setMinMax(point);
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
