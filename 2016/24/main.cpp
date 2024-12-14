#include "../../common.h"

struct Context {
    Grid<bool> space;
    Set<Coord> targets;
    Coord start;
    Vector<Coord> points;

    void parse(const Vector<String> &lines) {
        int y{0};
        for (auto &line : lines) {
            int x{0};
            for (char c : line) {
                if (c == '.') {
                    space[y][x] = true;
                }
                if (c >= '0' && c <= '9') {
                    space[y][x] = true;

                    if (c > '0') {
                        targets.insert({x, y});
                    } else {
                        start = {x, y};
                    }

                    points.push_back({x, y});
                }

                ++x;
            }
            ++y;
        }
    }

    struct Node {
        Coord pos;
        int steps;

        bool operator<(const Node &other) const {
            return steps < other.steps;
        }
    };

    int getMinSteps(const Coord from, const Coord to) {
        int minSteps{10000};
        PriorityQueue<Node> q;
        Grid<int> visited;

        auto firstNode = Node{from, 0};
        q.push(firstNode);

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();
            int x = node.pos.x;
            int y = node.pos.y;

            if (node.pos == to) {
                if (node.steps < minSteps) {
                    minSteps = node.steps;
                }
            } else {
                if (node.steps < minSteps) {
                    auto ncoords = {Coord{x + 1, y}, Coord{x - 1, y}, Coord{x, y + 1}, Coord{x, y - 1}};

                    for (const auto &ncoord : ncoords) {
                        if (space[ncoord.y][ncoord.x]) {
                            auto &v = visited[ncoord.y][ncoord.x];
                            if (v == 0 || node.steps < v) {
                                v = node.steps;
                                q.push({ncoord, node.steps + 1});
                            }
                        }
                    }
                }
            }
        }

        return minSteps;
    }

    struct Node2 {
        Coord pos;
        Set<Coord> targets;
        int steps;

        bool operator<(const Node2 &other) const {
            return steps < other.steps;
        }
    };

    void run() {
        Map<Coord, Map<Coord, int>> distances;
        for (const auto &a : points) {
            for (const auto &b : points) {
                if (a != b) {
                    int minSteps = getMinSteps(a, b);
                    /* log << a.x << ", " << a.y << " to " << b.x << ", " << b.y << " = " << minSteps << endl; */
                    distances[a][b] = minSteps;
                    distances[b][a] = minSteps;
                }
            }
        }

        {
            int minSteps{10000};
            PriorityQueue<Node2> q;

            auto firstNode = Node2{start, targets, 0};
            q.push(firstNode);

            while (!q.empty()) {
                auto node = q.top();
                q.pop();

                if (node.targets.empty()) {
                    if (node.steps < minSteps) {
                        minSteps = node.steps;
                    }
                } else {
                    if (node.steps < minSteps) {
                        for (auto &target : node.targets) {
                            auto copy = node;

                            copy.targets.erase(target);
                            copy.steps += distances[node.pos][target];
                            copy.pos = target;

                            q.push(copy);
                        }
                    }
                }
            }

            log << "part1: " << minSteps << endl;
        }

        {
            int minSteps{10000};
            PriorityQueue<Node2> q;

            auto firstNode = Node2{start, targets, 0};
            q.push(firstNode);

            while (!q.empty()) {
                auto node = q.top();
                q.pop();

                if (node.targets.empty() && node.pos == start) {
                    if (node.steps < minSteps) {
                        minSteps = node.steps;
                    }
                } else {
                    if (node.steps < minSteps) {
                        for (auto &target : node.targets) {
                            auto copy = node;

                            copy.targets.erase(target);
                            copy.steps += distances[node.pos][target];
                            copy.pos = target;

                            q.push(copy);
                        }

                        if (node.targets.empty()) {
                            auto copy = node;
                            copy.steps += distances[node.pos][start];
                            copy.pos = start;
                            q.push(copy);
                        }
                    }
                }
            }

            log << "part2: " << minSteps << endl;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx{};
    ctx.parse(lines);

    ctx.run();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
