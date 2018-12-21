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
template <class T> using Grid = Map<int, Map<int, T>>;

struct Coord {
    int x;
    int y;
};

struct Context {
    Grid<char> map;
    int minx{10000};
    int miny{10000};
    int maxx{-10000};
    int maxy{-10000};

    void computeBounds() {
        for (auto &kvy : map) {
            for (auto &kvx : kvy.second) {
                minx = std::min(kvx.first, minx);
                maxx = std::max(kvx.first, maxx);
            }
            miny = std::min(kvy.first, miny);
            maxy = std::max(kvy.first, maxy);
        }
    }

    void finalize() {
        for (int y{miny}; y <= maxy; ++y) {
            for (int x{minx}; x <= maxx; ++x) {
                if (map[y][x] == 0) {
                    map[y][x] = '#';
                }
            }
        }

        map[0][0] = 'X';
    }

    void draw() {
        for (int x{minx}; x <= maxx + 2; ++x) {
            std::cout << '#';
        }
        std::cout << std::endl;
        for (int y{miny}; y <= maxy; ++y) {
            std::cout << '#';
            for (int x{minx}; x <= maxx; ++x) {
                std::cout << map[y][x];
            }
            std::cout << '#' << std::endl;
        }
        for (int x{minx}; x <= maxx + 2; ++x) {
            std::cout << '#';
        }
        std::cout << std::endl;
    }

    int process(int startx, int starty, const std::string &str, const int start = 0) {
        int x{startx};
        int y{starty};
        for (int i{start}; i < str.length(); ++i) {
            char c = str[i];

            map[y][x] = '.';
            switch (c) {
            case 'W':
                map[y][x - 1] = '|';
                x -= 2;
                break;
            case 'E':
                map[y][x + 1] = '|';
                x += 2;
                break;
            case 'N':
                map[y - 1][x] = '-';
                y -= 2;
                break;
            case 'S':
                map[y + 1][x] = '-';
                y += 2;
                break;
            case '(':
                i = process(x, y, str, i + 1);
                break;
            case '|':
                x = startx;
                y = starty;
                /* process(startx, starty, str, c + 1); */
                break;
            case ')':
                return i;
                break;
            }
        }

        return str.length();
    }

    using Path = Vector<Coord>;

    struct BfsNode {
        Coord p;
        int distance;
    };

    int bfs(const Coord &from, const Coord &to) {
        static const int dx[4] = {0, -1, 1, 0};
        static const int dy[4] = {-1, 0, 0, 1};
        static Grid<bool> visited;
        for (auto &kv : visited) {
            for (auto &kv2 : kv.second) {
                kv2.second = false;
            }
        }

        std::queue<BfsNode> q;
        q.push(BfsNode{{from.x, from.y}, 0});
        visited[from.x][from.y] = true;
        int distance = 1000000;

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            const auto &p = node.p;

            if (p.x == to.x && p.y == to.y) {
                if (node.distance < distance) {
                    distance = node.distance;
                }
            }

            for (int i = 0; i < 4; ++i) {
                int nx = p.x + dx[i] * 2;
                int ny = p.y + dy[i] * 2;
                int sx = p.x + dx[i];
                int sy = p.y + dy[i];
                if (!visited[nx][ny]) {
                    if (map[ny][nx] == '.' && map[sy][sx] != '#') {
                        q.push({{nx, ny}, node.distance + 1});
                        visited[nx][ny] = true;
                    }
                }
            }
        }

        return distance;
    }

    void compute() {
        int maxDist{-1000};
        int count{0};
        Coord start{0, 0};
        for (int y{miny}; y <= maxy; ++y) {
            for (int x{minx}; x <= maxx; ++x) {
                if (map[y][x] == '.') {
                    int m = bfs(start, {x, y});
                    if (m >= 1000) {
                        ++count;
                    }
                    maxDist = std::max(m, maxDist);
                }
            }
        }

        std::cout << "part1: " << maxDist << std::endl;
        std::cout << "part2: " << count << std::endl;
    }
};

void processString(const std::string &str) {
    Context ctx;
    ctx.map[0][0] = 'X';
    ctx.process(0, 0, str);
    ctx.computeBounds();
    ctx.finalize();
    ctx.draw();
    ctx.compute();
}

void process(const char *file) {
    std::ifstream f(file);
    std::string line;
    std::string str;

    while (std::getline(f, line)) {
        processString(line);
    }

    f.close();
}

int main(int argc, char *argv[]) {
    processString("^WNE$");
    processString("^ENWWW(NEEE|SSE(EE|N))$");
    processString("^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))");
    processString("^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$");

    if (argc == 2) {
        process(argv[1]);
    }

    return 0;
}
