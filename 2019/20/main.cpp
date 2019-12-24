#include "../../common.h"

struct Maze {
    Vector<String> map;
    int width;
    int height;

    Map<Point, Point> portalLinks;
    Map<String, Point> portals;
    Vector<Point> outerPortals;
    Vector<Point> innerPortals;

    char safeGet(const int x, const int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return map[y][x];
        }

        return 0;
    }

    char get(const int x, const int y) const {
        return map[y][x];
    }

    char get(const Point &p) const {
        return map[p.y][p.x];
    }

    static inline bool isLetter(const char c) {
        return c >= 'A' && c <= 'Z';
    }

    bool addPortal(const String name, const int x, const int y) {
        char c = safeGet(x, y);

        if (c == '.') {

            if (portals.find(name) == portals.end()) {
                portals[name] = {x, y};

            } else {
                auto a = Point{x, y};
                auto b = portals[name];

                portalLinks[a] = b;
                portalLinks[b] = a;
            }

            if (x < 3 || x > width - 4 || y < 3 || y > height - 4) {
                outerPortals.push_back({x, y});
            } else {
                innerPortals.push_back({x, y});
            }

            return true;
        }

        return false;
    }

    void findPortal(const String name, const int x, const int y) {

        if (!addPortal(name, x - 1, y)) {
            if (!addPortal(name, x + 2, y)) {
                if (!addPortal(name, x, y - 1)) {
                    if (!addPortal(name, x, y + 2)) {
                        throw "Impossible";
                    }
                }
            }
        }
    }

    void init(const Vector<String> &lines) {
        map = lines;
        width = lines[0].length();
        height = lines.size();

        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                char c = get(x, y);

                if (isLetter(c)) {
                    String name;
                    name += c;

                    char c2 = safeGet(x + 1, y);

                    if (isLetter(c2)) {
                        name += c2;
                        findPortal(name, x, y);

                    } else {
                        c2 = safeGet(x, y + 1);

                        if (isLetter(c2)) {
                            name += c2;
                            findPortal(name, x, y);
                        }
                    }
                }
            }
        }
    }

    struct Node {
        Point pos;
        int steps;
        int level;

        bool operator<(const Node &other) const {
            if (level == other.level) {
                return steps < other.steps;
            }
            return level > other.level;
        }
    };

    template <int part> void run() {
        auto beg = portals["AA"];
        auto end = portals["ZZ"];
        int best = 100000000;
        PriorityQueue<Node> q;
        Map<Pair<Point, int>, int> visited;
        const Vector<Point> &deltas = {Point{0, 0}, Point{0, 1}, Point{0, -1}, Point{-1, 0}, Point{1, 0}};

        q.push({beg, 0, 0});

        while (!q.empty()) {
            const auto node = q.top();
            const auto &pos = node.pos;
            q.pop();

            auto &vs = visited[{pos, node.level}];
            if (vs != 0 && vs <= node.steps) {
                continue;
            } else {
                vs = node.steps;
            }

            if (node.steps < best) {

                if (pos == end && node.level == 0) {
                    best = node.steps;
                }

                for (const auto &delta : deltas) {
                    const auto &npos = pos + delta;
                    if (get(npos) == '.') {
                        auto copy = node;
                        copy.pos = npos;
                        copy.steps++;
                        q.push(copy);
                    }
                }

                if constexpr (part == 1) {
                    auto it = portalLinks.find(pos);
                    if (it != portalLinks.end()) {
                        auto copy = node;
                        copy.pos = it->second;
                        copy.steps++;
                        q.push(copy);
                    }
                } else {
                    auto it = std::find(innerPortals.begin(), innerPortals.end(), pos);
                    if (it != innerPortals.end()) {
                        auto it = portalLinks.find(pos);
                        if (it != portalLinks.end()) {
                            auto copy = node;
                            copy.pos = it->second;
                            copy.steps++;
                            copy.level++;
                            q.push(copy);
                        }
                    } else if (node.level > 0) {
                        auto it = std::find(outerPortals.begin(), outerPortals.end(), pos);
                        if (it != outerPortals.end()) {
                            auto it = portalLinks.find(pos);
                            if (it != portalLinks.end()) {
                                auto copy = node;
                                copy.pos = it->second;
                                copy.steps++;
                                copy.level--;
                                q.push(copy);
                            }
                        }
                    }
                }
            }
        }

        log << "part" << part << ": " << best << endl;
    }
};

template<int part>
void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Maze maze;
    maze.init(lines);
    maze.run<part>();
}

int main() {
    process<1>("sample.txt");
    process<1>("sample2.txt");
    process<2>("sample3.txt");
    process<1>("input.txt");
    process<2>("input.txt");
    return 0;
}
