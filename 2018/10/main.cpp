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

struct Point {
    int x, y;
    int vx, vy;

    void update() {
        x += vx;
        y += vy;
    }
};

void process(const char *file) {
    std::ifstream f(file);
    std::string line, str;
    std::vector<Point> points;
    char c;

    while (std::getline(f, line)) {
        std::istringstream iss{line};
        Point p;
        iss.ignore(10);
        iss >> p.x >> c >> p.y >> c;
        iss.ignore(11);
        iss >> p.vx >> c >> p.vy;
        points.push_back(p);
    }
    f.close();

    int i{0};

    while (i < 1000000) {

        auto limitx = std::minmax_element(points.begin(), points.end(), [](auto &a, auto &b) { return a.x < b.x; });
        auto limity = std::minmax_element(points.begin(), points.end(), [](auto &a, auto &b) { return a.y < b.y; });

        if (limity.second->y - limity.first->y < 20) {
            std::cout << "i=" << i << std::endl;

            for (int y{limity.first->y}; y <= limity.second->y + 1; ++y) {
                for (int x{limitx.first->x}; x <= limitx.second->x + 1; ++x) {
                    bool match{false};
                    for (auto &p : points) {
                        if (p.x == x && p.y == y) {
                            match = true;
                            break;
                        }
                    }

                    if (match) {
                        std::cout << "#";
                    } else {
                        std::cout << ".";
                    }
                }

                std::cout << std::endl;
            }
        }
        for (auto &p : points) {
            p.update();
        }

        ++i;
    }
}

int main() {
    process("input2.txt");
    process("input.txt");

    return 0;
}
