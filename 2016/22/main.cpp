#include "../../common.h"

struct Coord {
    int x;
    int y;
    bool operator==(const Coord &other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const Coord &other) const {
        return y == other.y ? x < other.x : y < other.y;
    }
};

struct Node {
    /* int x, y; */
    unsigned char size;
    unsigned char used;
    /* bool importantData{false}; */
    int available() const {
        return size - used;
    }

    void moveTo(Node &other) {
        other.used += used;
        used = 0;
        /* other.importantData |= importantData; */
        /* importantData = false; */
    }

    bool operator==(const Node &other) const {
        return size == other.size && used == other.used;
    }

    bool operator<(const Node &other) const {
        return used + 1000 * size < other.used + 1000 * other.size;
    }
};

struct Context {
    char width;
    char height;
    Vector<Node> nodes;

    void parse(String line) {
        if (line[0] != '/') {
            return;
        }

        Node node;
        InputStringStream iss{line};
        String path;
        char c;

        iss >> path >> (int &)node.size >> c >> (int &)node.used;

        InputStringStream iss2{path};

        constexpr int len = sizeof("/dev/grid/node-") - 1;
        iss2.ignore(len);

        /* iss2 >> c >> node.x >> c >> c >> node.y; */

        nodes.push_back(node);
    }

    void run() {
        height = nodes.size() / width;
        int count{0};
        for (auto &node : nodes) {
            for (auto &node2 : nodes) {
                if (isViable(node, node2)) {
                    ++count;
                }
            }
        }

        log << "part1: " << count << endl;

        part2();
    }

    struct BfsNode {
        Coord loc;
        int steps;

        bool operator<(const BfsNode &other) const {
            return steps < other.steps;
        }
    };

    // I admit I had to get help on this one...
    // https://www.reddit.com/r/adventofcode/comments/5jor9q/2016_day_22_solutions/

    void part2() {
        static Array<Coord, 4> dirs = {Coord{0, -1}, Coord{0, 1}, Coord{-1, 0}, Coord{1, 0}};
        short minSteps{2000};

        Map<Coord, int> visitedSteps;

        PriorityQueue<BfsNode> q;
        q.push({Coord{width - 1, 0}, 0});

        auto targetNode = nodes[getIndex(width - 1, 0)];

        while (!q.empty()) {
            const auto bfsnode = q.top();
            q.pop();
            const auto &loc = bfsnode.loc;
            const auto &node = nodes[getIndex(loc)];

            auto &vs = visitedSteps[loc];
            if (vs != 0 && vs <= bfsnode.steps) {
                continue;
            } else {
                vs = bfsnode.steps;
            }

            if (node.used == 0) {
                if (bfsnode.steps < minSteps) {
                    minSteps = bfsnode.steps;
                }

            } else {
                if (bfsnode.steps < minSteps) {

                    for (int i{0}; i < 4; ++i) {
                        const auto &dir = dirs[i];
                        Coord ncoord{loc.x + dir.x, loc.y + dir.y};

                        if (ncoord.x >= 0 && ncoord.x < width && ncoord.y >= 0 && ncoord.y < height) {
                            int idx = getIndex(ncoord.x, ncoord.y);
                            const auto &otherNode = nodes[idx];

                            if (otherNode.size < 100) {
                                q.push({ncoord, bfsnode.steps + 1});
                            }
                        }
                    }
                }
            }
        }

        // minSteps is minimal distance from target to empty.
        // now let's add the 'turtle' movement

        minSteps += (width - 2) * 5;

        log << "part2: " << minSteps << endl;
    }

    int getIndex(const int x, const int y) const {
        return x * height + y;
    }

    int getIndex(const Coord coord) const {
        return coord.x * height + coord.y;
    }

    static bool isViable(const Node &a, const Node &b) {
        if (a.used == 0)
            return false;

        if (&a == &b) {
            return false;
        }

        if (a.used > b.available()) {
            return false;
        }

        return true;
    }

    static bool isViableFast(const Node &a, const Node &b) {
        if (a.used == 0)
            return false;

        if (a.used > b.available()) {
            return false;
        }

        return true;
    }
};

void process(const String filename, const char i) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx{i};

    for (auto &line : lines) {
        ctx.parse(line);
    }

    ctx.run();
}

int main() {
    process("sample.txt", 3);
    process("input.txt", 33);
    return 0;
}
