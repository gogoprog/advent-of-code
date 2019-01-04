#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <vector>

template <class K, class V> using Map = std::map<K, V>;
template <class V> using Vector = std::vector<V>;
template <class V> using Set = std::set<V>;
template <class T> using Grid = Map<int, Map<int, T>>;
using ull = unsigned long long;

struct Point {
    int x;
    int y;

    bool operator==(const Point &other) const { return x == other.x && y == other.y; }
};

struct Context {
    int depth;
    Point target;
    mutable Grid<bool> cached;
    mutable Grid<int> cacheValue;

    int cachedGeologicIndex(const Point &p) const {
        if (cached[p.x][p.y]) {
            return cacheValue[p.x][p.y];
        }

        int v = geologicIndex(p);
        cacheValue[p.x][p.y] = v;
        cached[p.x][p.y] = true;

        return v;
    }

    int geologicIndex(const Point &p) const {
        if (p.x == 0 && p.y == 0) {
            return 0;
        }
        if (p == target) {
            return 0;
        }

        if (p.y == 0) {
            return p.x * 16807;
        }

        if (p.x == 0) {
            return p.y * 48271;
        }

        return erosionLevel({p.x - 1, p.y}) * erosionLevel({p.x, p.y - 1});
    }

    inline int erosionLevel(const Point &p) const { return (cachedGeologicIndex(p) + depth) % 20183; }

    inline int risk(const Point &p) const {
        int m = erosionLevel(p) % 3;
        return m;
    }

    enum { R = 0, W = 1, N = 2 };

    int risk() const {
        int r{0};
        for (int y{0}; y <= target.y; ++y) {
            for (int x{0}; x <= target.x; ++x) {
                r += risk({x, y});
            }
        }

        return r;
    }

    enum class Tool { Neither, ClimbingGear, Torch };

    Vector<Tool> tools{Tool::Neither, Tool::ClimbingGear, Tool::Torch};

    Map<Tool, std::array<bool, 3>> allowedTools = {{Tool::Neither, {false, true, true}},
                                                   {Tool::ClimbingGear, {true, true, false}},
                                                   {Tool::Torch, {true, false, true}}};

    struct BfsNode {
        Point p;
        int distance;
        Tool tool;
        int r;
    };

    int bfs() {
        const auto from = Point{0, 0};
        const auto to = target;
        static const int dx[] = {0, -1, 1, 0};
        static const int dy[] = {-1, 0, 0, 1};
        Map<Tool, Grid<int>> visited;

        std::queue<BfsNode> q;
        auto fnode = BfsNode{from, 0, Tool::Torch, risk(from)};
        q.push(fnode);
        visited[Tool::Torch][from.x][from.y] = 1;
        int distance = 100000;

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            const auto &p = node.p;

            if (p.x == to.x && p.y == to.y) {
                if (node.tool == Tool::Torch) {
                    if (node.distance < distance) {
                        distance = node.distance;
                    }
                }
            }

            if (node.distance < distance) {
                for (int i{0}; i < 4; ++i) {
                    Point np = {p.x + dx[i], p.y + dy[i]};
                    int nx = np.x;
                    int ny = np.y;
                    if (np.x >= 0 && np.y >= 0) {
                        if (!visited[node.tool][nx][ny] || visited[node.tool][nx][ny] > node.distance + 1) {
                            int r = risk(np);
                            if (allowedTools[node.tool][r]) {
                                q.push({np, node.distance + 1, node.tool, r});
                                visited[node.tool][nx][ny] = node.distance + 1;
                            }
                        }
                    }
                }

                for (auto tool : tools) {
                    if (tool != node.tool) {
                        if (!visited[tool][p.x][p.y] || visited[tool][p.x][p.y] > node.distance + 7) {
                            if (allowedTools[tool][node.r]) {
                                q.push({p, node.distance + 7, tool, node.r});
                                visited[tool][p.x][p.y] = node.distance + 7;
                            }
                        }
                    }
                }
            }
        }

        return distance;
    }
};

void process(const int depth, const Point target) {
    Context ctx{depth, target};
    std::cout << "total risk level is " << ctx.risk() << std::endl;

    std::cout << "rescue time: " << ctx.bfs() << std::endl;
}

int main(int argc, char *argv[]) {
    /* process(510, {10, 10}); */
    /* process(11739, {11, 718}); */
    process(5616, {10, 785});

    return 0;
}
