#include "../../common.h"

struct Space {
    Vector<String> lines;

    Map<Point, int> sightMap;
    Vector<Point> asteroidPoints;

    Map<Point, float> angles;

    void process() {
        for (int y = 0; y < lines.size(); y++) {
            auto &line = lines[y];
            for (int x = 0; x < line.size(); x++) {
                char c = line[x];

                if (c == '#') {
                    asteroidPoints.push_back({x, y});
                }
            }
        }

        for (auto &p : asteroidPoints) {
            sightMap[p] = countSight(p);
        }

        auto it = std::max_element(sightMap.begin(), sightMap.end(),
                                   [](const auto &a, const auto &b) { return a.second < b.second; });

        log << "best is " << it->first << " with " << it->second << endl;

        laser(it->first);
    }

    int countSight(const Point &inputPoint) {
        int r{0};

        for (auto &p : asteroidPoints) {
            if (p != inputPoint) {
                auto line = Line{p, inputPoint};

                int count{0};
                for (auto &other : asteroidPoints) {
                    if (other != p && other != inputPoint) {

                        if (isPointOnLineSegment(other, line)) {
                            ++count;
                        }
                    }
                }

                if (count == 0) {
                    r++;
                }
            }
        }

        return r;
    }

    void laser(const Point inputPoint) {

        struct Boom {
            float angle;
            Point target;
        };

        Vector<Boom> booms;

        for (auto &p : asteroidPoints) {
            if (p != inputPoint) {
                auto line = Line{p, inputPoint};

                int count{0};
                for (auto &other : asteroidPoints) {
                    if (other != p && other != inputPoint) {

                        if (isPointOnLineSegment(other, line)) {
                            ++count;
                        }
                    }
                }

                float angle = std::atan2(p.second - inputPoint.second, p.first - inputPoint.first);

                if (angle < 0) {
                    angle += M_PI * 2;
                }

                angle += M_PI / 2;

                if (angle > M_PI * 2) {
                    angle -= M_PI * 2;
                }

                angle += M_PI * 2 * count;

                booms.push_back({angle, p});
            }
        }

        std::sort(booms.begin(), booms.end(), [](const auto &a, const auto &b) { return a.angle < b.angle; });

        log << booms[199].target << endl;
        log << booms[199].target.first * 100 + booms[199].target.second << endl;
    }

    static bool isPointOnLineSegment(const Point p, const Line line) {
        int x1 = line.first.first;
        int x2 = line.second.first;
        int y1 = line.first.second;
        int y2 = line.second.second;
        int x = p.first;
        int y = p.second;
        bool first;
        if (y1 == y2) {
            return ((x1 < x && x < x2) || (x2 < x && x < x1)) && y == y1;
        } else if (x1 == x2) {
            return x1 == x && ((y1 < y && y < y2) || (y2 < y && y < y1));
        } else {
            first = std::abs((float)(x - x1) / (x2 - x1) - (float)(y - y1) / (y2 - y1)) < 0.0000001;
        }
        if (first) {
            return ((x1 < x && x < x2) || (x2 < x && x < x1)) && ((y1 < y && y < y2) || (y2 < y && y < y1));
        }

        return false;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    Space space{lines};

    space.process();
}

int main() {
    /* process("sample.txt"); */
    /* process("sample3.txt"); */
    process("sample2.txt");
    process("input.txt");
    return 0;
}
