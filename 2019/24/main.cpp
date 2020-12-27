#include "../../common.h"

template <bool recursive> struct System {
    using Bugs = Map<Point, bool>;
    using BugsMap = Map<int, Bugs>;
    mutable BugsMap bugsMap;
    Vector<Bugs> history;
    int size{5};

    mutable int min{0}, max{0};

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

    bool hasBug(const int x, const int y, const int depth) const {

        if constexpr (recursive) {
            if (x == -1) {
                return hasBug(1, 2, depth - 1);
            }
            if (x == size) {
                return hasBug(3, 2, depth - 1);
            }
            if (y == -1) {
                return hasBug(2, 1, depth - 1);
            }
            if (y == size) {
                return hasBug(2, 3, depth - 1);
            }
            if (x == 2 && y == 2) {
                return false;
            }
        }

        if (x < 0 || y < 0 || x >= size || y >= size)
            return false;

        auto r = bugsMap[depth][{x, y}];

        if (r) {
            max = std::max(depth, max);
            min = std::min(depth, min);
        }

        return r;
    }

    int countAdjacent(const Point point, const int depth) const {
        static const Point deltas[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        int r{0};
        int d{0};

        for (auto &delta : deltas) {
            auto coord = point + delta;

            if constexpr (recursive) {
                if (coord == Point{2, 2}) {
                    int c{0};
                    switch (d) {
                        case 0:
                            for (int y{0}; y < size; ++y) {
                                if (hasBug(0, y, depth + 1)) {
                                    c++;
                                }
                            }

                            break;
                        case 1:
                            for (int y{0}; y < size; ++y) {
                                if (hasBug(size - 1, y, depth + 1)) {
                                    c++;
                                }
                            }

                            break;
                        case 2:
                            for (int x{0}; x < size; ++x) {
                                if (hasBug(x, 0, depth + 1)) {
                                    c++;
                                }
                            }

                            break;
                        case 3:
                            for (int x{0}; x < size; ++x) {
                                if (hasBug(x, size - 1, depth + 1)) {
                                    c++;
                                }
                            }

                            break;
                    }

                    r += c;
                    d++;
                    continue;
                }
            }

            if (hasBug(coord.x, coord.y, depth)) {
                r++;
            }

            d++;
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

    void process2() {
        BugsMap futureBugsMap;
        for (int d{min - 1}; d <= max + 1; ++d) {
            auto &futureBugs = futureBugsMap[d];

            for (int y{0}; y < size; ++y) {
                for (int x{0}; x < size; ++x) {
                    if (x == 2 && y == 2)
                        continue;

                    Point p{x, y};

                    int count = countAdjacent(p, d);

                    if (hasBug(x, y, d)) {
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
        }

        bugsMap = futureBugsMap;
    }

    void print(const int depth = 0) const {
        log << "Depth " << depth << endl;

        for (int y{0}; y < size; ++y) {
            for (int x{0}; x < size; ++x) {

                if constexpr (recursive) {
                    if (x == 2 && y == 2) {
                        log << "?";
                        continue;
                    }
                }

                if (hasBug(x, y, depth)) {
                    log << "#";
                } else {
                    log << ".";
                }
            }

            log << endl;
        }
    }

    void print2() const {
        for (auto &kv : bugsMap) {
            print(kv.first);
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
        int r{0};
        for (auto &kv : bugsMap) {
            auto &bugs = kv.second;

            for (auto &kv2 : bugs) {
                if (kv2.second) {
                    r++;
                }
            }
        }
        return r;
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

        for (int i{0}; i < minutes; ++i) {
            system.process2();
        }

        log << "Part2: " << system.count() << endl;
    }
}

int main() {
    process("sample.txt", 10);
    process("input.txt", 200);
    return 0;
}
