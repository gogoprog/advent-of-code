// I needed a hint to complete this one and the hint was: "A single 3-dimensional bfs"

#include "../../common.h"

using PMap = Vector<String>;
const Vector<Point> deltas = {Point{0, -1}, Point{1, 0}, Point{0, 1}, Point{-1, 0}};

struct Id {
    Point pos;
    ulong keys;

    bool operator<(const Id &other) const {
        if (pos == other.pos) {
            return keys < other.keys;
        }
        return pos < other.pos;
    }
};

struct Node {
    Id id;
    int steps;

    inline bool walk(const Point &npos, PMap &map) {
        char c = map[npos.y][npos.x];
        bool can_walk = false;

        switch (c) {
            case '.':
            case '@':
                can_walk = true;

            default:
                if (c >= 'a' && c <= 'z') {
                    can_walk = true;
                    auto key = c - 'a';
                    id.keys |= (1 << key);
                } else if (c >= 'A' && c <= 'Z') {
                    auto key = (c - 'A');
                    can_walk = id.keys & (1 << key);
                }
        }

        if (can_walk) {
            id.pos = npos;
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

        map = lines;
    }

    void run() {
        PriorityQueue<Node> q;

        Node n{{startPos, 0}};
        q.push(n);

        int best{std::numeric_limits<int>::max()};

        ulong targetKeys = 0;

        for (uint k = 0; k < totalKeys; ++k) {
            targetKeys |= (1 << k);
        }

        Map<Id, int> visited;

        while (!q.empty()) {
            const auto node{q.top()};
            q.pop();

            auto &vs = visited[node.id];

            if (vs != 0 && vs <= node.steps) {
                continue;
            } else {
                vs = node.steps;
            }

            if (node.steps < best) {
                const auto &id = node.id;

                for (const auto &delta : deltas) {
                    const auto &npos = id.pos + delta;
                    auto copy = node;

                    if (copy.walk(npos, map)) {

                        if (copy.id.keys == targetKeys) {
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
