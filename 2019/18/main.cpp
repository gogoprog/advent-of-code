// I needed a hint to complete this one and the hint was: "A single 3-dimensional bfs"

#include "../../common.h"

using PMap = Map<Point, char>;
using Keys = std::bitset<32>;
const Vector<Point> deltas = {Point{0, -1}, Point{1, 0}, Point{0, 1}, Point{-1, 0}};

struct Node {
    Point pos;
    int steps;
    Keys keys;

    Node() = default;

    Node &operator=(const Node &other) = default;

    inline bool canWalk(const Point &npos, PMap &map) const {
        char c = map[npos];
        switch (c) {
            case '#':
                return false;
            case '.':
            case '@':
                return true;

            default:
                if (c >= 'a' && c <= 'z') {
                    return true;
                } else if (c >= 'A' && c <= 'Z') {
                    auto key = (c - 'A');
                    return keys[key];
                }
        }

        return false;
    }

    inline bool walk(const Point &npos, PMap &map) {
        char c = map[npos];
        bool can_walk = false;

        switch (c) {
            case '.':
            case '@':
                can_walk = true;

            default:
                if (c >= 'a' && c <= 'z') {
                    can_walk = true;
                    auto key = c - 'a';
                    keys[key] = true;
                } else if (c >= 'A' && c <= 'Z') {
                    auto key = (c - 'A');
                    can_walk = keys[key];
                }
        }

        if (can_walk) {
            pos = npos;
            ++steps;
        }

        return can_walk;
    }

    bool operator<(const Node &other) const {
        return steps < other.steps;
    }
};

struct Level {
    Point startPos;
    uint totalKeys;
    PMap map;

    void load(const Vector<String> &lines) {
        int y = 0;
        int x = 0;

        totalKeys = 0;

        for (auto &line : lines) {
            x = 0;
            for (auto c : line) {
                map[{x, y}] = c;

                if (c == '@') {
                    startPos = {x, y};
                }

                if (c >= 'a' && c <= 'z') {
                    totalKeys++;
                }
                ++x;
            }
            ++y;
        }
    }

    void run() {
        PriorityQueue<Node> q;

        Node n{startPos, 0};
        q.push(n);

        int best{std::numeric_limits<int>::max()};

        Map<Pair<Point, ulong>, int> visited;

        while (!q.empty()) {
            const auto node{q.top()};
            q.pop();

            auto &vs = visited[{node.pos, node.keys.to_ulong()}];

            if (vs != 0 && vs <= node.steps) {
                continue;
            } else {
                vs = node.steps;
            }

            if (node.steps < best) {

                for (const auto &delta : deltas) {
                    const auto & npos = node.pos + delta;
                    auto copy = node;

                    if (copy.walk(npos, map)) {

                        if (copy.keys.count() == totalKeys) {
                            best = copy.steps;
                        } else {
                            q.push(copy);
                        }
                    }
                }
            }
        }

        log << "part1: " << best << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Level level;

    level.load(lines);

    level.run();
}

int main() {
    process("sample.txt");
    process("sample2.txt");
    process("sample3.txt");
    process("sample4.txt");
    process("sample5.txt");
    process("input.txt");
    return 0;
}
