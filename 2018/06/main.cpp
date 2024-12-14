#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <limits>

struct Point {
    int x;
    int y;
};

int computeManhattan(const Point &a, const Point &b) { return std::abs(a.x - b.x) + std::abs(a.y - b.y); }

int main() {
    /* const int maxDist{32}; */
    const int maxDist{10000};
    std::ifstream f("input.txt");
    std::string line, str;
    std::vector<Point> points;
    Point min{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()},
        max{std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};

    std::map<int, std::map<int, int>> grid;

    while (std::getline(f, line)) {
        Point point;
        char c;
        std::istringstream iss(line);

        iss >> point.x >> c >> point.y;

        points.push_back(point);

        if (point.x < min.x) {
            min.x = point.x;
        }
        if (point.y < min.y) {
            min.y = point.y;
        }
        if (point.x > max.x) {
            max.x = point.x;
        }
        if (point.y > max.y) {
            max.y = point.y;
        }
    }
    f.close();

    std::set<int> infiniteIndexes{};

    int part2Total{0};

    for (int y{min.y}; y <= max.y; ++y) {
        for (int x{min.x}; x <= max.x; ++x) {
            Point gridPoint{x, y};
            int closest{-1};
            bool ties{false};
            int dist{std::numeric_limits<int>::max()};
            int total{0};

            for (int p{0}; p < points.size(); ++p) {
                auto &point = points[p];
                auto d = computeManhattan(point, gridPoint);
                total += d;
                if (d < dist) {
                    dist = d;
                    ties = false;
                    closest = p;
                } else if (d == dist) {
                    ties = true;
                }
            }

            if (ties) {
                grid[x][y] = -1;
            } else {
                grid[x][y] = closest;

                if (x == min.x || x == max.x || y == min.x || y == max.y) {
                    infiniteIndexes.insert(closest);
                }
            }

            if (total < maxDist) {
                part2Total++;
            }
        }
    }

    {
        std::vector<int> areas;

        for (int p{0}; p < points.size(); ++p) {
            if (infiniteIndexes.find(p) == infiniteIndexes.end()) {
                int area{0};
                for (int y{min.y}; y <= max.y; ++y) {
                    for (int x{min.x}; x <= max.x; ++x) {
                        if (grid[x][y] == p) {
                            ++area;
                        }
                    }
                }
                areas.push_back(area);
            }
        }

        std::cout << "part1: " << *std::max_element(areas.begin(), areas.end()) << std::endl;
    }

    std::cout << "part2: " << part2Total << std::endl;

    return 0;
}
