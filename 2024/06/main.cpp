#include "../../common_fast.h"

template <> struct std::hash<Pair<Coord, int>> {
    std::size_t operator()(const Pair<Coord, int> &v) const {
        return v.first.x + 10000 * v.first.y + v.second * 1000000;
    }
};

struct Context {

    static constexpr Array<Coord, 8> deltas = {Coord{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    StringViews lines;
    int width, height;
    Coord start;

    inline bool isValid(Coord coord) const {
        return coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height;
    }

    inline bool isWalkable(Coord coord) const {
        char c = getChar(coord);
        return c == '.' || c == '^';
    }

    inline char getChar(Coord coord) const {
        return lines[coord.y][coord.x];
    }

    void parse(auto _lines) {
        lines = {};
        rs::copy(_lines, std::back_inserter(lines));
        width = lines[0].size();
        height = lines.size();

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (getChar({x, y}) == '^') {
                    start = {x, y};
                    return;
                }
            }
        }
    }

    void go(auto lines) {

        parse(lines);

        auto testLoop = [&](const auto start, const auto _dir, const auto extra) {
            UnorderedMap<Pair<Coord, int>, bool> visited;
            auto current = start;
            int dir = _dir;

            while (true) {
                auto delta = deltas[dir];
                auto nextpos = current + delta;

                if (!isValid(nextpos)) {
                    break;
                } else {
                    if (isWalkable(nextpos) && nextpos != extra) {
                        current = nextpos;
                    } else {
                        dir = (dir + 1) % 4;

                        auto key = Pair<Coord, int>{current, dir};

                        if (visited.contains(key)) {
                            return true;
                        }

                        visited[key] = true;
                    }
                }
            }

            return false;
        };

        Set<Coord> visited;
        Set<Coord> solutions;
        auto current = start;
        int dir = 0;

        visited.insert(current);

        while (true) {
            auto delta = deltas[dir];
            auto nextpos = current + delta;

            if (!isValid(nextpos)) {
                break;
            } else {
                if (isWalkable(nextpos)) {

                    auto extra = nextpos;

                    if (extra != start && !visited.contains(extra)) {
                        if (!solutions.contains(extra)) {
                            auto t = testLoop(current, (dir + 1) % 4, extra);

                            if (t) {
                                solutions.insert(extra);
                            }
                        }
                    }

                    current = nextpos;
                    visited.insert(current);

                } else {
                    dir = (dir + 1) % 4;
                }
            }
        }

        auto result = visited.size();
        auto result2 = solutions.size();

        log << "Part1: " << result << endl;
        log << "Part2: " << result2 << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context context;
        context.go(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
