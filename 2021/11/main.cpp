#include "../../common.h"

struct Context {
    static constexpr Array<Point, 8> directions = {Point{0, -1}, {1, 0},  {0, 1},  {-1, 0},
                                                   {-1, -1},     {-1, 1}, {1, -1}, {1, 1}};
    Grid<int> grid;
    int width;
    int height;
    Vector<Point> positions;

    void init(const Vector<String> &data) {
        width = data[0].size();
        height = data.size();

        for (int y{0}; y < height; ++y) {
            for (int x{0}; x < width; ++x) {
                grid[x][y] = data[y][x] - '0';
                positions.push_back({x, y});
            }
        }
    }

    bool exists(const Point &p) {
        auto x = p.x;
        auto y = p.y;
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    int &get(const Point &p) {
        return grid[p.x][p.y];
    }

    void increaseAdjacents(const Point &pos) {

        for (auto &delta : directions) {
            auto new_pos = pos + delta;

            if (exists(new_pos)) {
                get(new_pos)++;
            }
        }
    }

    void draw() {
        for (int y{0}; y < height; ++y) {
            for (int x{0}; x < width; ++x) {
                log << grid[x][y];
            }

            log << endl;
        }
    }

    void run() {
        auto flashes{0};
        Set<Point> flashing_positions;

        auto i = 0;
        while (true) {

            /* log << "Step " << i << endl; */
            /* draw(); */

            for (auto &pos : positions) {
                get(pos)++;
            }

            bool didFlash{true};
            flashing_positions.clear();
            while (didFlash) {
                didFlash = false;

                auto flashing_view = positions | rv::filter([this](auto &p) { return get(p) > 9; });

                for (auto &pos : flashing_view) {

                    if (flashing_positions.find(pos) == flashing_positions.end()) {
                        increaseAdjacents(pos);
                        didFlash = true;

                        flashing_positions.insert(pos);
                    }
                }
            }

            if (flashing_positions.size() == positions.size()) {
                log << "Part2: " << i + 1 << endl;
                break;
            }

            flashes += flashing_positions.size();

            for (auto &pos : flashing_positions) {
                get(pos) = 0;
            }

            ++i;

            if (i == 100) {

                log << "Part1: " << flashes << endl;
            }
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context context;
    context.init(lines);
    context.run();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
