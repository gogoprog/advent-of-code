#include "../../common.h"

static const Vector<Point> &ortho_deltas = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
static const Vector<Point> &all_deltas = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
enum Direction { N, S, W, E, SE, NE, SW, NW };

struct Elf {
    Coord position;
    int direction{-1};
    Coord nextPosition;
};

using Adjacents = Array<bool, 8>;

Array<Function<bool(const Adjacents &adjacents)>, 4> considerFuncs = {
    [](auto adjacents) { return (!adjacents[N] && !adjacents[NE] && !adjacents[NW]); },
    [](auto adjacents) { return (!adjacents[S] && !adjacents[SE] && !adjacents[SW]); },
    [](auto adjacents) { return (!adjacents[W] && !adjacents[NW] && !adjacents[SW]); },
    [](auto adjacents) { return (!adjacents[E] && !adjacents[NE] && !adjacents[SE]); },
};

struct Context {
    Map<Coord, bool> grid;
    Vector<Elf> elves;

    void parse(const Strings &lines) {
        for (int y = 0; y < lines.size(); y++) {
            for (int x = 0; x < lines[0].size(); x++) {

                if (lines[y][x] == '#') {
                    Coord pos{x, y};
                    grid[pos] = true;
                    elves.push_back({});
                    elves.back().position = pos;
                }
            }
        }
    }

    void refreshGrid() {
        grid.clear();
        for (auto e : elves) {
            grid[e.position] = true;
        }
    }

    bool round(const int r) {

        bool result = {false};

        refreshGrid();

        for (auto &elf : elves) {
            bool alone{true};
            Array<bool, 8> adjacents;
            for (int d = 0; d < 8; ++d) {
                auto delta = all_deltas[d];
                auto pos = elf.position + delta;
                adjacents[d] = grid[pos];

                if (grid[pos]) {
                    alone = false;
                }
            }

            if (!alone) {
                elf.direction = -1;
                for (int i = 0; i < 4; ++i) {
                    auto d = (i + r) % 4;

                    if (considerFuncs[d](adjacents)) {
                        elf.direction = d;
                        break;
                    }
                }

                /* assert(elf.direction != -1); */
            } else {
                elf.direction = -1;
            }
        }

        Map<Coord, int> countMap;

        for (auto &elf : elves) {
            if (elf.direction != -1) {
                auto delta = all_deltas[elf.direction];
                auto npos = elf.position + delta;
                countMap[npos]++;
                elf.nextPosition = npos;
            }
        }

        for (auto &elf : elves) {
            if (elf.direction != -1) {
                if (countMap[elf.nextPosition] == 1) {
                    elf.position = elf.nextPosition;
                    result = true;
                }
            }
        }

        return result;
    }

    void print() {

        int size = 10;
        refreshGrid();

        for (int y = -size / 2; y < size; y++) {
            for (int x = -size / 2; x < size; x++) {
                auto pos = Coord{x, y};

                if (grid[pos]) {
                    log << '#';
                } else {
                    log << '.';
                }
            }

            log << endl;
        }
    }

    void part1() {
        auto result{0};

        for (int i{0}; i < 10; ++i) {
            /* log << i << endl; */
            round(i);
            /* print(); */
        }

        Point minPoint{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
        Point maxPoint{std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};

        for (auto &elf : elves) {
            auto &pos = elf.position;
            minPoint.x = std::min(pos.x, minPoint.x);
            minPoint.y = std::min(pos.y, minPoint.y);
            maxPoint.x = std::max(pos.x, maxPoint.x);
            maxPoint.y = std::max(pos.y, maxPoint.y);
        }

        auto w = maxPoint.x - minPoint.x + 1;
        auto h = maxPoint.y - minPoint.y + 1;

        result = w * h - elves.size();

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0};

        while (true) {
            if (!round(result)) {
                break;
            }
            result++;
        }

        log << "Part2: " << result + 1 << endl;
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
    /* process("sample2.txt"); */
    process("sample.txt");
    process("input.txt");
    return 0;
}
