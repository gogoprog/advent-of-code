#include "../../common.h"

template <bool recursive> struct System {
    using Bugs = Map<Point, bool>;
    mutable Map<int, Bugs> bugsMap;
    Vector<Bugs> history;
    int size{5};

    void parse(const Vector<String> &lines) {
        for (int y{0}; y < size; ++y) {
            auto &line = lines[y];
            for (int x{0}; x < size; ++x) {

                if (line[x] == '#') {
                    bugsMap[0][{x, y}] = true;
                }
            }
        }
    }

    int getBugCount(const int x, const int y, const int depth = 0) const {
        if (x < 0 || y < 0 || x >= size || y >= size)
            return false;

        auto it = bugsMap[depth].find({x, y});
        if (it != bugsMap[depth].end()) {

            return 1;
        }

        return 0;
    }

    bool hasBug(const int x, const int y, const int depth) const {
        return getBugCount(x, y, depth) > 0;
    }

    int countAdjacent(const Point point, const int depth) const {
        static const Point deltas[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        int r{0};

        for (auto &delta : deltas) {
            auto coord = point + delta;

            r += getBugCount(coord.x, coord.y, depth);
        }
        return r;
    }

    bool process() {
        auto &bugs = bugsMap[0];
        history.push_back(bugs);
        Bugs futureBugs;

        for (int y{0}; y < size; ++y) {
            for (int x{0}; x < size; ++x) {
                Point p{x, y};

                int count = countAdjacent(p, 0);

                if (hasBug(x, y, 0)) {
                    if (count == 1) {
                        futureBugs[p] = true;
                    }
                } else {
                    if (count == 1 || count == 2) {
                        futureBugs[p] = true;
                    }
                }
            }
        }

        bugs = futureBugs;

        auto it = std::find(history.begin(), history.end(), bugs);
        if (it != history.end()) {
            return true;
        }

        return false;
    }

    void print() const {
        for (int y{0}; y < size; ++y) {
            for (int x{0}; x < size; ++x) {

                if (hasBug(x, y, 0)) {
                    log << "#";
                } else {
                    log << ".";
                }
            }

            log << endl;
        }
    }

    lli compute() const {
        lli r{0};
        lli v{1};
        for (int y{0}; y < size; ++y) {
            for (int x{0}; x < size; ++x) {

                if (hasBug(x, y, 0)) {
                    r += v;
                }

                v = v << 1;
            }
        }

        return r;
    }

    lli count() const {
        return 0;
    }
};

void process(const String filename, const int minutes) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    {
        System<false> system{};
        system.parse(lines);

        system.print();

        while (true) {
            if (system.process()) {
                system.print();
                log << "Part1: " << system.compute() << endl;
                break;
            }
        }
    }
    {
        System<true> system{};
        system.parse(lines);

        system.print();

        for (int i{0}; i < minutes; ++i) {
            system.process();
        }

        log << "Part2: " << system.count() << endl;
    }
}

int main() {
    process("sample.txt", 10);
    process("input.txt", 200);
    return 0;
}
