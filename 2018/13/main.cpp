#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

using Map = std::vector<std::string>;

enum class Direction { N, E, S, W };

struct Cart {
    size_t x;
    size_t y;
    Direction dir;
    int lastIntersectionDir{2};

    void advance(const Map &map) {
        char current = map[y][x];

        switch (current) {
        case '-':
            break;
        case '|':
            break;

        case '/': {
            switch (dir) {
            case Direction::E:;
                dir = Direction::N;
                break;
            case Direction::W:
                dir = Direction::S;
                break;
            case Direction::S:
                dir = Direction::W;
                break;
            case Direction::N:
                dir = Direction::E;
                break;
            }

        } break;
        case '\\': {
            switch (dir) {
            case Direction::E:;
                dir = Direction::S;
                break;
            case Direction::W:
                dir = Direction::N;
                break;
            case Direction::S:
                dir = Direction::E;
                break;
            case Direction::N:
                dir = Direction::W;
                break;
            }
        } break;

        case '+': {
            int d = static_cast<int>(dir);
            lastIntersectionDir = (lastIntersectionDir + 1) % 3;

            d += lastIntersectionDir - 1;

            while (d < 0) {
                d += 4;
            }

            d %= 4;
            dir = static_cast<Direction>(d);

        } break;
        }

        switch (dir) {
        case Direction::E:
            x += 1;
            break;
        case Direction::W:
            x -= 1;
            break;
        case Direction::S:
            y += 1;
            break;
        case Direction::N:
            y -= 1;
            break;
        }
    }
};

void process(const char *file) {
    std::ifstream f(file);
    std::string line;
    Map lines;

    while (std::getline(f, line)) {
        lines.push_back(line);
    }

    f.close();

    size_t w{lines[0].length()};
    size_t h{lines.size()};
    std::vector<Cart> carts;

    for (size_t y{0}; y < h; ++y) {
        for (size_t x{0}; x < w; ++x) {
            char c = lines[y][x];

            switch (c) {
            case '>': {
                carts.push_back({x, y, Direction::E});
                lines[y][x] = '-';
            } break;
            case '<': {
                carts.push_back({x, y, Direction::W});
                lines[y][x] = '-';
            } break;
            case '^': {
                carts.push_back({x, y, Direction::N});
                lines[y][x] = '|';
            } break;
            case 'v': {
                carts.push_back({x, y, Direction::S});
                lines[y][x] = '|';
            } break;
            }
        }
    }

    bool first{true};
    std::set<int> crashedIndexes;

    while (carts.size() > 1) {

        int i{0};
        for (i = 0; i < carts.size(); ++i) {
            auto &cart = carts[i];
            cart.advance(lines);

            auto fx = cart.x;
            auto fy = cart.y;
            int j{0};
            for (auto &otherCart : carts) {
                if (i != j) {
                    if (cart.x == otherCart.x && cart.y == otherCart.y) {
                        crashedIndexes.insert(i);
                        crashedIndexes.insert(j);
                        if (first) {
                            std::cout << "part1: " << fx << "," << fy << std::endl;
                            first = false;
                        }
                        break;
                    }
                }
                ++j;
            }

            for (int r = carts.size() - 1; r >= 0; --r) {
                if (crashedIndexes.find(r) != crashedIndexes.end()) {
                    carts.erase(carts.begin() + r);
                    if (r <= i) {
                        --i;
                    }
                }
            }
            crashedIndexes.clear();
        }
    }

    if (carts.size() == 1) {
        std::cout << "part2: " << carts[0].x << "," << carts[0].y << std::endl;
    }
}

int main() {
    process("input2.txt");
    process("input3.txt");
    process("input.txt");

    return 0;
}
