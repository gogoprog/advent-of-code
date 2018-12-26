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
    int z;
};
int computeManhattan(const Point &a, const Point &b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
}

struct Nanobot {
    Point position;
    int radius;
    bool rangeClipsWithCube(const Point &a, const Point &b) const {
        int dist{0};

        if (position.x < a.x)
            dist += a.x - position.x;
        else if (position.x > b.x)
            dist += position.x - b.x;
        if (position.y < a.y)
            dist += a.y - position.y;
        else if (position.y > b.y)
            dist += position.y - b.y;
        if (position.z < a.z)
            dist += a.z - position.z;
        else if (position.z > b.z)
            dist += position.z - b.z;

        return dist <= radius;
    }
};

void process(const char *file) {
    std::ifstream f(file);
    std::string line;
    std::string str;
    char c;

    Vector<Nanobot> nanobots;

    while (std::getline(f, line)) {
        std::istringstream iss{line};
        iss.ignore(5);

        nanobots.push_back({});
        auto &nanobot = nanobots.back();

        iss >> nanobot.position.x >> c >> nanobot.position.y >> c >> nanobot.position.z;
        iss.ignore(5);
        iss >> nanobot.radius;
    }

    f.close();

    {
        const auto it =
            std::max_element(nanobots.begin(), nanobots.end(), [](auto &a, auto &b) { return a.radius < b.radius; });

        const auto &strongest = (*it);

        int count = std::count_if(nanobots.begin(), nanobots.end(), [&](auto &n) {
            int distance = computeManhattan(n.position, strongest.position);
            return distance <= strongest.radius;
        });

        std::cout << "part1: " << count << std::endl;
    }
    {
        Point min, max;
        min.x = std::min_element(nanobots.begin(), nanobots.end(),
                                 [](auto &a, auto &b) { return a.position.x < b.position.x; })
                    ->position.x;
        max.x = std::max_element(nanobots.begin(), nanobots.end(),
                                 [](auto &a, auto &b) { return a.position.x < b.position.x; })
                    ->position.x;
        min.y = std::min_element(nanobots.begin(), nanobots.end(),
                                 [](auto &a, auto &b) { return a.position.y < b.position.y; })
                    ->position.y;
        max.y = std::max_element(nanobots.begin(), nanobots.end(),
                                 [](auto &a, auto &b) { return a.position.y < b.position.y; })
                    ->position.y;
        min.z = std::min_element(nanobots.begin(), nanobots.end(),
                                 [](auto &a, auto &b) { return a.position.z < b.position.z; })
                    ->position.z;
        max.z = std::max_element(nanobots.begin(), nanobots.end(),
                                 [](auto &a, auto &b) { return a.position.z < b.position.z; })
                    ->position.z;

        // https://gist.github.com/pdewacht/fa7aa7e60952c6d67956599d6d4af360

        int s = 1;
        while (s < std::max(max.x - min.x, std::max(max.y - min.y, max.z - min.z))) {
            s *= 2;
        }

        struct Node {
            int score;
            int dist;
            int s;
            Point pos;

            bool operator<(const Node &other) const { return score > other.score; }
        };

        std::priority_queue<Node> q;

        auto add = [&](const int x1, const int y1, const int z1) {
            auto x2 = x1 + s - 1;
            auto y2 = y1 + s - 1;
            auto z2 = z1 + s - 1;
            int in_range{0};
            for (auto &b : nanobots) {
                if (b.rangeClipsWithCube({x1, y1, z1}, {x2, y2, z2})) {
                    ++in_range;
                }
            }
            if (in_range > 0) {
                int dist = std::min(abs(x1), abs(x2)) + std::min(abs(y1), abs(y2)) + std::min(abs(z1), abs(z2));
                q.push(Node{-in_range, dist, s, {x1, y1, z1}});
            }
        };

        add(min.x, min.y, min.z);

        while (!q.empty()) {
            auto node = q.top();
            q.pop();
            auto &p = node.pos;
            s = node.s;

            if (s == 1) {
                std::cout << "part2: " << std::endl;
                std::cout << p.x << ", " << p.y << ", " << p.z << std::endl;
                std::cout << "bots in range: " << -node.score << std::endl;
                std::cout << "dist: " << node.dist << std::endl;
                break;
            }

            int x{p.x};
            int y{p.y};
            int z{p.z};

            s /= 2;
            add(x, y, z);
            add(x, y, z + s);
            add(x, y + s, z);
            add(x, y + s, z + s);
            add(x + s, y, z);
            add(x + s, y, z + s);
            add(x + s, y + s, z);
            add(x + s, y + s, z + s);
        }
    }
}

int main(int argc, char *argv[]) {
    process(argv[1]);

    return 0;
}
