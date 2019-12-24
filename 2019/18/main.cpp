// I needed a hint to complete this one and the hint was: "A single 3-dimensional bfs"
// Hint for part2: "active bot"

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

struct Id2 {
    Array<Point, 4> positions;
    ulong keys;

    bool operator<(const Id2 &other) const {
        if (positions[0] == other.positions[0]) {
            if (positions[1] == other.positions[1]) {
                if (positions[2] == other.positions[2]) {
                    if (positions[3] == other.positions[3]) {
                        return keys < other.keys;
                    }
                    return positions[3] < other.positions[3];
                }
                return positions[2] < other.positions[2];
            }
            return positions[1] < other.positions[1];
        }
        return positions[0] < other.positions[0];
    }
};

struct Node2 {
    Id2 id;
    int steps;
    int activeBot = -1;

    inline bool walk(const int n, const Point &npos, PMap &map) {
        char c = map[npos.y][npos.x];
        bool can_walk = false;

        activeBot = n;

        switch (c) {
            case '.':
            case '@':
                can_walk = true;

            default:
                if (c >= 'a' && c <= 'z') {
                    can_walk = true;
                    auto key = c - 'a';
                    if (!(id.keys & (1 << key))) {
                        activeBot = -1;
                    }
                    id.keys |= (1 << key);
                } else if (c >= 'A' && c <= 'Z') {
                    auto key = (c - 'A');
                    can_walk = id.keys & (1 << key);
                }
        }

        if (can_walk) {
            id.positions[n] = npos;
            ++steps;
        }

        return can_walk;
    }

    bool operator<(const Node2 &other) const {
        return steps < other.steps;
    }
};

struct Level {
    Point startPos;
    Vector<Point> startPositions;
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
                    startPositions.push_back(startPos);
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
        if (startPositions.size() == 1) {
            run1();
        } else {
            run2();
        }
    }

    void run1() {
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

    void run2() {
        int best{std::numeric_limits<int>::max()};
        PriorityQueue<Node2> q;
        Map<Id2, int> visited;

        Node2 n{{{startPositions[0], startPositions[1], startPositions[2], startPositions[3]}, 0}};
        q.push(n);

        ulong targetKeys = 0;

        for (uint k = 0; k < totalKeys; ++k) {
            targetKeys |= (1 << k);
        }

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

                if (node.activeBot == -1) {

                    for (int n = 0; n < 4; n++) {
                        for (const auto &delta : deltas) {
                            const auto &npos = id.positions[n] + delta;
                            auto copy = node;

                            if (copy.walk(n, npos, map)) {

                                if (copy.id.keys == targetKeys) {
                                    best = copy.steps;
                                } else {
                                    q.push(copy);
                                }
                            }
                        }
                    }
                } else {
                    auto n = node.activeBot;
                    for (const auto &delta : deltas) {
                        const auto &npos = id.positions[n] + delta;
                        auto copy = node;

                        if (copy.walk(n, npos, map)) {

                            if (copy.id.keys == targetKeys) {
                                best = copy.steps;
                            } else {
                                q.push(copy);
                            }
                        }
                    }
                }
            }
        }

        log << "part2: " << best << endl;
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
    process("sample6.txt");
    process("sample7.txt");
    process("sample8.txt");
    process("input2.txt");
    return 0;
}
