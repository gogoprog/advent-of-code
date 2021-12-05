#include "../../common.h"
#include <algorithm>

struct VentLine {
    Point a, b;

    bool isAxisAligned() const {
        return a.x == b.x || a.y == b.y;
    }

    bool isHorizontal() const {
        return a.y == b.y;
    }

    bool isVertical() const {
        return a.x == b.x;
    }

    Point getStep() const {
        auto dx = b.x - a.x;
        auto dy = b.y - a.y;

        if (dx) {
            dx /= std::abs(dx);
        }

        if (dy) {
            dy /= std::abs(dy);
        }

        return {dx, dy};
    }
};

template <int part> void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    Vector<VentLine> ventLines;

    for (auto &line : lines) {
        char c;
        ventLines.push_back({});
        auto &ventLine = ventLines.back();
        InputStringStream iss{line};

        iss >> ventLine.a.x >> c >> ventLine.a.y >> c >> c >> ventLine.b.x >> c >> ventLine.b.y;
    }

    Map<Point, int> grid;
    Map<Point, int> grid2;

    for (auto &line : ventLines) {
        if (part == 2 || line.isAxisAligned()) {
            auto p = line.a;
            auto step = line.getStep();

            do {
                grid[p]++;
                p += step;
            } while (p != line.b);

            grid[p]++;
        }
    }

    auto r = std::count_if(grid.begin(), grid.end(), [](const auto &kv) { return kv.second >= 2; });

    log << "Part" << part << ": " << r << endl;
}

int main() {
    process<1>("sample.txt");
    process<2>("sample.txt");
    process<1>("input.txt");
    process<2>("input.txt");
    return 0;
}
