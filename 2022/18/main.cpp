#include "../../common.h"

constexpr Array<Vector3, 6> deltas = {Vector3{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};

struct Context {
    Vector<Point3> cubes;

    void parse(const Strings &lines) {
        for (auto &line : lines) {
            auto p = parsePoint3(line);
            cubes.push_back(p);
        }
    }

    void part1() {

        Vector<int> touches;
        touches.resize(cubes.size());

        for (int i{0}; i < cubes.size(); ++i) {
            touches[i] = 0;

            for (int j{0}; j < cubes.size(); ++j) {

                if (i != j) {
                    auto delta = cubes[j] - cubes[i];

                    if (delta.manhattan() == 1) {
                        touches[i]++;
                    }
                }
            }
        }

        auto result{0};
        for (auto t : touches) {
            result += (6 - t);
        }

        log << "Part1: " << result << endl;
    }

    bool isValid(const Point3 p) const {
        for (int i = 0; i < 3; ++i) {
            auto v = p.coords[i];
            if (v < -1 || v > 20) {
                return false;
            }
        }

        return true;
    }

    bool isCube(const Point3 p) const {
        for (auto &cube : cubes) {
            if (cube == p)
                return true;
        }

        return false;
    }

    void part2() {
        Map<Point3, bool> exterior;

        Map<Point3, bool> visited;

        PriorityQueue<Point3> q;

        q.push({0, 0, 0});

        while (!q.empty()) {
            auto pos = q.top();
            q.pop();

            if (visited[pos]) {
                continue;
            }

            visited[pos] = true;
            exterior[pos] = !isCube(pos);

            if (exterior[pos]) {
                for (auto &delta : deltas) {
                    auto npos = pos + delta;

                    if (isValid(npos)) {
                        q.push(npos);
                    }
                }
            }
        }

        auto result{0};
        for (auto &kv : exterior) {
            auto pos = kv.first;
            if (kv.second) {
                for (auto &delta : deltas) {
                    auto npos = pos + delta;
                    if (isValid(npos)) {
                        if (!exterior[npos]) {
                            result++;
                        }
                    }
                }
            }
        }

        log << "Part2: " << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        Context context;
        context.parse(lines);
        context.part1();
    }
    {
        Context context;
        context.parse(lines);
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
