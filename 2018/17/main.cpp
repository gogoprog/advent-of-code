#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

template <class K, class V> using Map = std::map<K, V>;
template <class T> using Grid = Map<int, Map<int, T>>;

int minx{100000}, maxx{-10000}, miny{100000}, maxy{-10000};

struct Point {
    int x;
    int y;
};

struct Leak {
    int x;
    int y;
    bool complete{false};
};

template <class T> void draw(T &grid) {
    for (int y{miny}; y <= maxy; ++y) {
        for (int x{minx}; x <= maxx; ++x) {
            std::cout << grid[y][x];
        }
        std::cout << std::endl;
    }
}

void process(const char *file) {
    std::ifstream f(file);
    std::string line;
    std::string str;
    char terma, termb, c;
    Grid<char> grid;
    int a, ra, rb;
    int ominy;
    const Point source{500, 0};
    minx = 100000;
    maxx = -10000;
    miny = 100000;
    maxy = -10000;

    while (std::getline(f, line)) {
        std::istringstream iss{line};

        iss >> terma >> c >> a >> c;
        iss >> termb >> c >> ra >> c >> c >> rb;

        if (terma == 'x') {
            int x = a;

            for (int y{ra}; y <= rb; ++y) {
                grid[y][x] = '#';

                minx = std::min(minx, x - 10);
                maxx = std::max(maxx, x + 10);
                miny = std::min(miny, y);
                maxy = std::max(maxy, y);
            }
        } else {
            int y = a;

            for (int x{ra}; x <= rb; ++x) {
                grid[y][x] = '#';
                minx = std::min(minx, x - 10);
                maxx = std::max(maxx, x + 10);
                miny = std::min(miny, y);
                maxy = std::max(maxy, y);
            }
        }
    }

    f.close();

    ominy = miny;
    minx = std::min(minx, source.x);
    maxx = std::max(maxx, source.x);
    miny = std::min(miny, source.y);
    maxy = std::max(maxy, source.y);

    for (int y{miny}; y <= maxy; ++y) {

        for (int x{minx}; x <= maxx; ++x) {
            if (grid[y][x] == 0) {
                grid[y][x] = '.';
            }
        }
    }

    std::vector<Leak> leaks{{source.x, source.y}};
    std::vector<Leak> new_leaks;
    Grid<bool> leakMap;
    grid[source.y][source.x] = '+';
    bool leaking{true};

    while (leaking) {
        leaking = false;
        for (auto &leak : leaks) {
            if (maxx - minx < 80) {
                draw(grid);
            }
            if (!leak.complete) {
                int y = leak.y;

                while (grid[y][leak.x] == '|' || grid[y][leak.x] == '+') {
                    ++y;
                    if (y > maxy) {
                        leak.complete = true;
                        break;
                    }
                }

                if (leak.complete) {
                    continue;
                }

                char c = grid[y][leak.x];

                if (c == '.') {
                    grid[y][leak.x] = '|';
                    leaking = true;
                } else {
                    --y;

                    bool bl{false}, br{false};
                    int rx, lx;

                    for (int x{leak.x}; x >= minx; --x) {
                        if (grid[y][x] == '#') {
                            bl = true;
                            break;
                        }

                        if (grid[y + 1][x] == '#' || grid[y + 1][x] == '~') {
                            if (grid[y][x] == '.') {
                                grid[y][x] = '|';
                            }
                        } else {
                            lx = x;
                            break;
                        }
                    }

                    for (int x{leak.x}; x <= maxx; ++x) {
                        if (grid[y][x] == '#') {
                            br = true;
                            break;
                        }

                        if (grid[y + 1][x] == '#' || grid[y + 1][x] == '~') {
                            if (grid[y][x] == '.') {
                                grid[y][x] = '|';
                            }
                        } else {
                            rx = x;
                            break;
                        }
                    }

                    if (bl && br) {
                        int x = leak.x;
                        while (grid[y][x] == '|') {
                            grid[y][x] = '~';
                            leaking = true;
                            x++;
                        }
                        x = leak.x - 1;
                        while (grid[y][x] == '|') {
                            grid[y][x] = '~';
                            leaking = true;
                            x--;
                        }
                    } else {
                        if (!br) {
                            int x = rx;
                            if (grid[y][x] == '.') {
                                grid[y][x] = '|';
                                leaking = true;
                            }

                            if (!leakMap[y][x]) {
                                new_leaks.push_back({x, y});
                                leakMap[y][x] = true;
                            }
                        }
                        if (!bl) {
                            int x = lx;
                            if (grid[y][x] == '.') {
                                grid[y][x] = '|';
                                leaking = true;
                            }

                            if (!leakMap[y][x]) {
                                new_leaks.push_back({x, y});
                                leakMap[y][x] = true;
                            }
                        }
                    }
                }
            }
        }

        if (new_leaks.size()) {
            leaks.insert(leaks.end(), new_leaks.begin(), new_leaks.end());
            new_leaks.clear();
        }
    }

    if (maxx - minx < 80) {
        draw(grid);
    }

    {
        unsigned long long count{0};
        for (int y{ominy}; y <= maxy; ++y) {
            for (int x{minx}; x <= maxx; ++x) {
                char c = grid[y][x];
                if (c == '|' || c == '~') {
                    ++count;
                }
            }
        }

        std::cout << "part1: " << count << std::endl;
    }
    {
        unsigned long long count{0};
        for (int y{ominy}; y <= maxy; ++y) {
            for (int x{minx}; x <= maxx; ++x) {
                char c = grid[y][x];
                if (c == '~') {
                    ++count;
                }
            }
        }

        std::cout << "part2: " << count << std::endl;
    }
}

int main(int argc, char *argv[]) {
    process(argv[1]);

    return 0;
}
