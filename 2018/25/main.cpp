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
    int w;
};

inline int computeManhattan(const Point &a, const Point &b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z) + std::abs(a.w - b.w);
}

struct Constellation {
    Vector<Point> points;
    /* Point min{std::numeric_limits<int>::max()}, max{std::numeric_limits<int>::min()}; */

    bool match(const Point &point) const {
        for (auto &p : points) {
            if (computeManhattan(p, point) <= 3) {
                return true;
            }
        }

        return false;
    }

    void add(const Point &point) { points.push_back(point); }
};

struct Context {
    Vector<Constellation> constellations;

    void add(const Point &point) {

        for (auto &constellation : constellations) {

            if (constellation.match(point)) {
                constellation.add(point);
                return;
            }
        }

        constellations.push_back({});
        auto &c = constellations.back();
        c.add(point);
    }

    void log() {
        int total{0};
        for (auto &con : constellations) {
            std::cout << "Constellation has " << con.points.size() << std::endl;
            total += con.points.size();
        }

        std::cout << total << " points total" << std::endl;
    }

    void merge() {
        bool merged{true};

        while (merged) {
            merged = false;

            for (auto &con : constellations) {
                int otherIndex{0};
                for (auto &other : constellations) {
                    if (&other != &con) {
                        for (auto &p : con.points) {
                            if (other.match(p)) {

                                con.points.insert(con.points.end(), other.points.begin(), other.points.end());

                                constellations.erase(constellations.begin() + otherIndex);

                                merged = true;
                                break;
                            }
                        }

                        if (merged) {
                            break;
                        }
                    }
                    otherIndex++;
                }

                if (merged) {
                    break;
                }
            }
        }
    }
};

void process(const char *file) {
    std::ifstream f(file);
    std::string line;
    char c;

    Vector<Point> points;

    while (std::getline(f, line)) {
        std::istringstream iss{line};
        points.push_back({});
        auto &p = points.back();

        iss >> p.x >> c >> p.y >> c >> p.z >> c >> p.w;
    }

    f.close();

    Context ctx;

    for (auto &p : points) {
        ctx.add(p);
    }

    ctx.merge();

    std::cout << "part1: " << ctx.constellations.size() << std::endl;
}

int main(int argc, char *argv[]) {
    process(argv[1]);

    return 0;
}
