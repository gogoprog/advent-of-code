#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>
#include <fstream>
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

using Lines = std::vector<std::string>;

template <class T> using Grid = Map<int, Map<int, T>>;

struct Coord {
    int x;
    int y;
};

using Path = Vector<Coord>;

struct Context;

struct Unit {
    Unit(Context &ctx) : ctx(ctx) {}
    Context &ctx;
    char team;
    int x;
    int y;
    int ix;
    int iy;
    int hp{200};
    int power{3};

    void moveTo(const int x, const int y);
    bool moveClosestAdjacentCoordsToEnemy(Vector<Path> &paths);
    void testCoord(Vector<Path> &paths, int &bestDistance, const int ox, const int oy);
    bool attack();
    void log();

    void reset(const int p) {
        power = p;
        hp = 200;
        x = -10;
        y = -10;
        moveTo(ix, iy);
    }
};

struct Context {
    friend class Unit;
    Grid<Unit *> positions;
    Grid<bool> space;
    Grid<bool> ispace;
    Vector<std::unique_ptr<Unit>> units;
    bool ended{false};
    int width;
    int height;
    int round{0};

    void log() {
        for (auto &unit : units) {
            std::cout << unit->team << " has " << unit->hp << std::endl;
        }
    }

    void reset(const int elf_power) {
        round = 0;
        ended = false;
        for (int y{0}; y < height; ++y) {
            for (int x{0}; x < width; ++x) {
                positions[y][x] = nullptr;
            }
        }
        for (auto &unit : units) {
            unit->reset(unit->team == 'E' ? elf_power : 3);
        }

        for (int y{0}; y < height; ++y) {
            for (int x{0}; x < width; ++x) {
                space[y][x] = ispace[y][x];
            }
        }
    }
    bool isFinished() const {
        char team = 0;
        bool finished = true;
        for (auto &unit : units) {
            if (unit->hp > 0) {
                if (team == 0) {
                    team = unit->team;
                } else {
                    if (team != unit->team) {
                        finished = false;
                        break;
                    }
                }
            }
        }

        return finished;
    }

    bool processRound() {
        orderedUnits.resize(0);

        for (int y{0}; y < height; ++y) {
            for (int x{0}; x < width; ++x) {
                auto u = positions[y][x];
                if (u && u->hp > 0) {
                    orderedUnits.push_back(u);
                }
            }
        }

        for (auto unit : orderedUnits) {
            if (unit->hp <= 0) {
                continue;
            }
            if (unit->moveClosestAdjacentCoordsToEnemy(paths)) {
            }
            if (unit->attack()) {
                ended = true;
                if (&*unit == orderedUnits.back()) {
                    ++round;
                }

                return true;
            }
        }

        ++round;
        return false;
    }

    void draw() {
        for (int y{0}; y < height; ++y) {
            for (int x{0}; x < width; ++x) {
                auto u = positions[y][x];
                if (u) {
                    std::cout << u->team;
                } else {
                    std::cout << (space[y][x] ? '.' : '#');
                }
            }
            std::cout << std::endl;
        }
    }

    inline bool isEmpty(const int x, const int y) { return space[y][x]; }

    struct BfsNode {
        int x;
        int y;
        int dist;
    };

    void bfs(Vector<Path> &paths, int &bestDistance, const Coord &from, const Coord &to) {
        static const int dx[4] = {0, -1, 1, 0};
        static const int dy[4] = {-1, 0, 0, 1};
        static Grid<bool> visited;
        for (auto &kv : visited) {
            for (auto &kv2 : kv.second) {
                kv2.second = false;
            }
        }

        std::queue<Path> q;
        q.push(Path{from});
        visited[from.x][from.y] = true;

        while (!q.empty()) {
            const auto path = q.front();
            q.pop();

            const auto p = path.back();
            const auto pathsize = path.size();
            if (p.x == to.x && p.y == to.y) {
                if (pathsize < bestDistance) {
                    bestDistance = path.size();
                    paths.clear();
                    paths.push_back(path);
                } else if (pathsize == bestDistance) {
                    paths.push_back(path);
                }
            }

            if (pathsize < bestDistance) {
                for (int i = 0; i < 4; ++i) {
                    int nx = p.x + dx[i];
                    int ny = p.y + dy[i];
                    if (!visited[nx][ny]) {
                        if (isEmpty(nx, ny)) {
                            Path newPath{path};

                            newPath.push_back({nx, ny});

                            q.push(newPath);
                            visited[nx][ny] = true;
                        }
                    }
                }
            }
        }
    }

    int outcome() const {
        unsigned long long allhp = std::accumulate(units.begin(), units.end(), 0, [](auto &a, auto &b) {
            if (b->hp > 0) {
                return a + b->hp;
            }
            return a;
        });

        return round * allhp;
    }

  private:
    Vector<Unit *> orderedUnits;
    Vector<Path> paths;
};

void Unit::moveTo(const int nx, const int ny) {
    ctx.positions[y][x] = nullptr;
    ctx.space[y][x] = true;
    x = nx;
    y = ny;
    ctx.positions[y][x] = this;
    ctx.space[y][x] = false;
}

void Unit::testCoord(Vector<Path> &paths, int &bestDistance, const int ox, const int oy) {
    if (ctx.isEmpty(ox, oy)) {
        ctx.bfs(paths, bestDistance, {x, y}, {ox, oy});
    }
}

bool Unit::moveClosestAdjacentCoordsToEnemy(Vector<Path> &paths) {
    int bestDistance = 100000;

    ctx.positions[y][x] = nullptr;
    ctx.space[y][x] = true;
    paths.clear();

    for (auto &unit : ctx.units) {
        if (&*unit != this) {
            if (unit->team != team) {
                int ox = unit->x;
                int oy = unit->y;

                testCoord(paths, bestDistance, ox + 1, oy);
                testCoord(paths, bestDistance, ox - 1, oy);
                testCoord(paths, bestDistance, ox, oy + 1);
                testCoord(paths, bestDistance, ox, oy - 1);
            }
        }
    }

    if (!paths.empty()) {

        if (paths[0].size() == 1) {
            moveTo(x, y);
            return false;
        }

        if (paths.size() > 1) {
            std::sort(paths.begin(), paths.end(), [&](auto &a, auto &b) {
                int ax, ay, bx, by;
                ax = a.back().x;
                ay = a.back().y;
                bx = b.back().x;
                by = b.back().y;
                if (ay == by) {
                    return ax < bx;
                }
                return ay < by;
            });
        }

        int nx, ny;
        nx = paths[0][1].x;
        ny = paths[0][1].y;
        moveTo(nx, ny);
        return true;

    } else {
        moveTo(x, y);
        return false;
    }
}

bool Unit::attack() {
    const int dx[4] = {0, -1, 1, 0};
    const int dy[4] = {-1, 0, 0, 1};
    Unit *enemy{nullptr};
    int bestHp = 10000;

    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        auto u = ctx.positions[ny][nx];
        if (u) {
            if (u->team != team) {
                if (u->hp < bestHp) {
                    bestHp = u->hp;
                    enemy = &*u;
                }
            }
        }
    }

    if (enemy) {
        enemy->hp -= power;

        if (enemy->hp <= 0) {
            enemy->moveTo(-10, -10);

            if (ctx.isFinished()) {
                return true;
            }
        }
    }

    return false;
}

void Unit::log() {}

void process(const char *file) {
    std::ifstream f(file);
    std::string line;
    Context ctx;

    int y{0};
    while (std::getline(f, line)) {
        ctx.width = line.length();
        for (int x{0}; x < line.length(); ++x) {
            char c = line[x];

            switch (c) {
            case '#':
                ctx.ispace[y][x] = false;
                break;
            case '.':
                ctx.ispace[y][x] = true;
                break;
            case 'G':
            case 'E':
                ctx.ispace[y][x] = false;
                ctx.units.push_back(std::make_unique<Unit>(ctx));
                auto &unit = *ctx.units.back();
                unit.ix = x;
                unit.iy = y;
                unit.team = c;
                break;
            }
        }

        ++y;
    }
    f.close();

    ctx.height = y;

    ctx.reset(3);
    while (!ctx.ended) {
        /* std::cout << "After round " << r << std::endl; */
        /* ctx.draw(); */
        /* ctx.log(); */
        /* ctx.units[1]->log(); */
        ctx.processRound();
    }

    /* ctx.draw(); */
    /* ctx.log(); */
    std::cout << "part1: " << ctx.round << " rounds, outcome is " << ctx.outcome() << std::endl;

    int elf_power{4};
    bool elfwon{false};
    int beforeelvescount = std::accumulate(ctx.units.begin(), ctx.units.end(), 0, [](auto &a, auto &b) {
        if (b->team == 'E') {
            return a + 1;
        }
        return a;
    });

    while (!elfwon) {
        ctx.reset(elf_power);
        while (!ctx.ended) {
            /* std::cout << "After round " << ctx.round << std::endl; */
            /* ctx.draw(); */
            /* ctx.log(); */
            ctx.processRound();
        }
        int afterelvescount = std::accumulate(ctx.units.begin(), ctx.units.end(), 0, [](auto &a, auto &b) {
            if (b->hp > 0 && b->team == 'E') {
                return a + 1;
            }
            return a;
        });

        if (beforeelvescount == afterelvescount) {
            /* ctx.draw(); */
            /* ctx.log(); */
            elfwon = true;
            std::cout << "part2: " << ctx.round << " rounds, outcome is  " << ctx.outcome() << ", attack is "
                      << elf_power << std::endl;
        }
        ++elf_power;
    }
}

int main() {
    /* process("test1.txt"); */
    /* process("test2.txt"); */
    /* process("test3.txt"); */
    /* process("input2.txt"); */
    process("input3.txt");
    /* process("input4.txt"); */
    process("input5.txt");
    /* process("input6.txt"); */
    /* process("input7.txt"); */
    /* process("input8.txt"); */
    process("input.txt");

    return 0;
}
