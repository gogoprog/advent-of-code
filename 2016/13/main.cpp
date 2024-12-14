#include "../../common.h"

uint count(uint n) {
    uint r{0};

    while (n) {
        r += n & 1;
        n >>= 1;
    }

    return r;
}

struct Context {
    int number;
    Coord target;

    bool isWall(const int x, const int y) {
        uint r = x * x + 3 * x + 2 * x * y + y + y * y;
        r += number;

        auto c = count(r);

        return c & 1;
    }

    void draw() {
        for (int j{0}; j < 10; ++j) {
            for (int i{0}; i < 10; ++i) {

                bool w = isWall(i, j);
                log << (w ? '#' : '.');
            }
            log << endl;
        }
    }

    struct Node {
        Coord coord;
        int steps;

        bool operator<(const Node &other) const { return steps < other.steps; }
    };

    void solve() {
        Coord start{1, 1};
        Grid<int> visitedSteps;
        int minSteps{10000};

        std::priority_queue<Node> q;
        q.push({start, 0});

        while (!q.empty()) {
            auto node = q.top();
            q.pop();

            const auto &coord = node.coord;

            if (!visitedSteps[coord.x][coord.y] || visitedSteps[coord.x][coord.y] > node.steps) {
                visitedSteps[coord.x][coord.y] = node.steps;

                if (coord == target) {
                    if (node.steps < minSteps) {
                        minSteps = node.steps;
                    }
                } else {
                    static Array<Coord, 4> dirs = {Coord{0, 1}, Coord{0, -1}, Coord{-1, 0}, Coord{1, 0}};

                    for (const auto &dir : dirs) {
                        int nx = coord.x + dir.x;
                        int ny = coord.y + dir.y;

                        if (!isWall(nx, ny)) {
                            q.push({Coord{nx, ny}, node.steps + 1});
                        }
                    }
                }
            }
        }

        log << minSteps << endl;
    }

    void part2() {
        Coord start{1, 1};
        Grid<int> visitedSteps;

        std::priority_queue<Node> q;
        q.push({start, 0});

        while (!q.empty()) {
            auto node = q.top();
            q.pop();

            const auto &coord = node.coord;

            if (!visitedSteps[coord.x][coord.y] || visitedSteps[coord.x][coord.y] > node.steps) {
                visitedSteps[coord.x][coord.y] = node.steps;

                if (node.steps <= 50) {
                    static Array<Coord, 4> dirs = {Coord{0, 1}, Coord{0, -1}, Coord{-1, 0}, Coord{1, 0}};

                    for (const auto &dir : dirs) {
                        int nx = coord.x + dir.x;
                        int ny = coord.y + dir.y;
                        if (nx >= 0 && ny >= 0) {
                            if (!isWall(nx, ny)) {
                                q.push({Coord{nx, ny}, node.steps + 1});
                            }
                        }
                    }
                }
            }
        }

        int r{0};
        for (auto &kv : visitedSteps) {

            for (auto &kv2 : kv.second) {
                if (kv2.second <= 50) {
                    r++;
                }
            }
        }

        log << "part2: " << r << endl;
    }
};

void process(const String filename, const int n, const Coord target) {
    log << "Processing " << filename << endl;

    Context ctx{n, target};

    ctx.solve();
    ctx.part2();
}

int main() {
    process("sample.txt", 10, {7, 4});
    process("input.txt", 1350, {31, 39});
    return 0;
}
