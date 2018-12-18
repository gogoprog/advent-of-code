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
template <class V> using Vector = std::vector<V>;
template <class T> using Grid = Map<int, Map<int, T>>;
using Area = Vector<std::string>;

struct Context {
    Area area;
    int width;
    int height;
    int minutes{0};
    bool ended{false};
    int interval{1};

    void init() {
        nextArea = area;
        width = area[0].length();
        height = area.size();
        history.push_back(area);
    }

    inline char getTile(const int x, const int y) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return area[y][x];
        }

        return 0;
    }

    inline void addTile(Vector<char> &result, const int x, const int y) {
        char c;

        c = getTile(x, y);
        if (c)
            result.push_back(c);
    }

    void fillAdjacent(Vector<char> &result, const int x, const int y) {
        addTile(result, x - 1, y);
        addTile(result, x + 1, y);
        addTile(result, x, y - 1);
        addTile(result, x, y + 1);
        addTile(result, x + 1, y + 1);
        addTile(result, x - 1, y + 1);
        addTile(result, x - 1, y - 1);
        addTile(result, x + 1, y - 1);
    }

    void processTile(const int x, const int y) {
        adjacents.clear();
        fillAdjacent(adjacents, x, y);
        char current = area[y][x];

        switch (current) {
        case '.': {
            int trees = std::count_if(adjacents.begin(), adjacents.end(), [](char c) { return c == '|'; });
            if (trees >= 3) {
                nextArea[y][x] = '|';
            } else {
                nextArea[y][x] = '.';
            }
        } break;
        case '|': {
            int lumberyards = std::count_if(adjacents.begin(), adjacents.end(), [](char c) { return c == '#'; });
            if (lumberyards >= 3) {
                nextArea[y][x] = '#';
            } else {
                nextArea[y][x] = '|';
            }
        } break;
        case '#': {
            int lumberyards = std::count_if(adjacents.begin(), adjacents.end(), [](char c) { return c == '#'; });
            int trees = std::count_if(adjacents.begin(), adjacents.end(), [](char c) { return c == '|'; });
            if (trees >= 1 && lumberyards >= 1) {
                nextArea[y][x] = '#';
            } else {
                nextArea[y][x] = '.';
            }
        } break;
        }
    }

    void update() {
        for (int y{0}; y < height; ++y) {
            for (int x{0}; x < width; ++x) {
                processTile(x, y);
            }
        }
        ++minutes;
        std::swap(nextArea, area);

        if (interval == 1) {
            int i{0};
            for (auto &pastArea : history) {
                if (pastArea == area) {
                    interval = minutes - i;
                    int target = i + (1000000000 - i) % interval;
                    area = history[target];
                    std::cout << "part2: " << value() << std::endl;
                    ended = true;
                    break;
                }
                ++i;
            }

            history.push_back(area);
        }
    }

    void draw() {
        std::cout << "After " << minutes << "m" << std::endl;
        for (auto &line : area) {
            std::cout << line << std::endl;
        }
        std::cout << std::endl;
    }

    unsigned long long value() {
        int lumberyards{0};
        int trees{0};
        for (int y{0}; y < height; ++y) {
            for (int x{0}; x < width; ++x) {
                char c = area[y][x];
                switch (c) {
                case '|':
                    ++trees;

                    break;
                case '#':
                    ++lumberyards;

                    break;
                }
            }
        }

        return trees * lumberyards;
    }

  private:
    Area nextArea;
    Vector<char> adjacents;
    Vector<Area> history;
};

void process(const char *file) {
    std::ifstream f(file);
    std::string line;
    Context ctx;

    while (std::getline(f, line)) {
        ctx.area.push_back(line);
    }

    f.close();

    ctx.init();

    while (!ctx.ended) {
        ctx.draw();
        ctx.update();
        if (ctx.minutes == 10) {
            std::cout << "part1: " << ctx.value() << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    process(argv[1]);

    return 0;
}
