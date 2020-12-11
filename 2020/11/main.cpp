#include "../../common.h"

struct System {
    Vector<String> lines;
    int width;
    int height;

    void init() {
        width = lines[0].length();
        height = lines.size();
    }

    bool process() {
        auto copy = lines;

        for (int y{0}; y < lines.size(); ++y) {
            for (int x{0}; x < width; ++x) {

                auto seat = lines[y][x];

                switch (seat) {
                    case 'L': {
                        auto count = countAdjacent(x, y, '#');
                        if (count == 0) {
                            copy[y][x] = '#';
                        }
                        break;
                    }
                    case '#': {
                        auto count = countAdjacent(x, y, '#');
                        if (count >= 4) {
                            copy[y][x] = 'L';
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        if (copy == lines) {
            return true;
        }

        lines = copy;

        return false;
    }

    void draw() {
        for (auto &line : lines) {
            log << line << endl;
        }
    }

    char getSeat(const int x, const int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return lines[y][x];
        }
        return 0;
    }

    int countAdjacent(const int x, const int y, const char which) {
        int result{0};

        if (getSeat(x - 1, y + 0) == which)
            result++;
        if (getSeat(x - 1, y + 1) == which)
            result++;
        if (getSeat(x - 1, y - 1) == which)
            result++;
        if (getSeat(x + 1, y + 0) == which)
            result++;
        if (getSeat(x + 1, y + 1) == which)
            result++;
        if (getSeat(x + 1, y - 1) == which)
            result++;
        if (getSeat(x - 0, y + 1) == which)
            result++;
        if (getSeat(x - 0, y - 1) == which)
            result++;

        return result;
    }

    int count(const char which) {
        int result{0};
        for (int y{0}; y < lines.size(); ++y) {
            for (int x{0}; x < width; ++x) {
                auto seat = lines[y][x];
                if (seat == which) {
                    ++result;
                }
            }
        }

        return result;
    }

    bool process2() {
        auto copy = lines;

        for (int y{0}; y < lines.size(); ++y) {
            for (int x{0}; x < width; ++x) {

                auto seat = lines[y][x];

                switch (seat) {
                    case 'L': {
                        auto count = countVisible(x, y, '#');
                        if (count == 0) {
                            copy[y][x] = '#';
                        }
                        break;
                    }
                    case '#': {
                        auto count = countVisible(x, y, '#');
                        if (count >= 5) {
                            copy[y][x] = 'L';
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        if (copy == lines) {
            return true;
        }

        lines = copy;

        return false;
    }

    int countVisible(const int x, const int y, const char which) {
        static const Vector<Point> &deltas = {Point{0, 1}, Point{0, -1}, Point{-1, 0}, Point{1, 0},
                                              Point{1, 1}, Point{1, -1}, Point{-1, 1}, Point{-1, -1}};

        Point start{x, y};

        int result{0};

        for (auto &delta : deltas) {
            auto current = start;
            char seat;

            do {
                current += delta;
                seat = getSeat(current.x, current.y);
                if (seat == which) {
                    result++;
                    break;
                }

            } while (seat == '.');
        }

        return result;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    {
        System system{lines};

        system.init();

        int i{0};
        while (true) {
            ++i;
            if (system.process()) {
                auto r = system.count('#');
                log << "Part1: " << r << endl;
                break;
            }
        }
    }
    {
        System system{lines};

        system.init();

        int i{0};
        while (true) {
            ++i;
            if (system.process2()) {
                auto r = system.count('#');
                log << "Part2: " << r << endl;
                break;
            }
        }
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
