#include "../../common.h"

template <typename T> std::tuple<Point, Point> getMinMax2(const Map<Point, T> &map) {
    Point minPoint{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
    Point maxPoint{std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};

    for (auto &kv : map) {

        if (!kv.second) {
            auto &pos = kv.first;
            minPoint.x = std::min(pos.x, minPoint.x);
            minPoint.y = std::min(pos.y, minPoint.y);
            maxPoint.x = std::max(pos.x, maxPoint.x);
            maxPoint.y = std::max(pos.y, maxPoint.y);
        }
    }

    return {minPoint, maxPoint};
}

template <bool blink> struct Context {
    String algo;
    Map<Point, bool> grid;
    Map<Point, bool> nextGrid;

    int index{0};

    void addGridLine(const String &line, const int y) {
        for (int x = 0; x < line.size(); ++x) {
            grid[{x, y}] = (line[x] == '#');
        }
    }

    bool getValue(const Point &p) {
        auto it = grid.find(p);

        if (it == grid.end()) {
            if constexpr (blink) {
                if ((index % 2)) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }

        return it->second;
    }

    uint getAlgoNumber(const Point &p) {
        std::bitset<9> bits;

        int i{0};
        for (int y = p.y - 1; y <= p.y + 1; ++y) {
            for (int x = p.x - 1; x <= p.x + 1; ++x) {
                auto p2 = Point{x, y};

                bits.set(8 - i, getValue(p2));
                ++i;
            }
        }

        return bits.to_ulong();
    }

    void step() {
        const auto [minPoint, maxPoint] = getMinMax(grid);

        int extra = 2;

        for (int y = minPoint.second - extra; y <= maxPoint.second + extra; y++) {
            for (int x = minPoint.first - extra; x <= maxPoint.first + extra; ++x) {
                auto p = Point{x, y};

                auto algon = getAlgoNumber(p);

                auto next_value = (algo[algon] == '#');
                nextGrid[p] = next_value;
            }
        }

        std::swap(grid, nextGrid);

        index++;
    }

    ull compute() {
        auto result = 0;
        const auto [minPoint, maxPoint] = getMinMax(grid);

        int intra = 0;

        for (int y = minPoint.second + intra; y <= maxPoint.second - intra; y++) {
            for (int x = minPoint.first + intra; x <= maxPoint.first - intra; ++x) {
                auto p = Point{x, y};

                if (grid[p]) {
                    result++;
                }
            }
        }

        return result;
    }

    void draw() {
        const auto [minPoint, maxPoint] = getMinMax(grid);
        for (int y = minPoint.second; y <= maxPoint.second; y++) {
            for (int x = minPoint.first; x <= maxPoint.first; ++x) {
                log << (grid[{x, y}] ? '#' : '.');
            }
            log << endl;
        }
    }
};

template <bool blink> void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context<blink> ctx{lines[0]};

    int y = 0;
    for (auto &line : lines | rv::drop(2)) {
        ctx.addGridLine(line, y);
        y++;
    }

    ctx.step();
    ctx.step();

    log << "Part1: " << ctx.compute() << endl;

    for (auto i : rs::iota_view(2, 50)) {
        ctx.step();
    }

    log << "Part2: " << ctx.compute() << endl;
}

int main() {
    process<false>("sample.txt");
    process<true>("input.txt");
    return 0;
}
