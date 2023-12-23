#include "../../common_fast.h"

constexpr Array<Coord, 4> dirs = {Coord{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

template <int STEPS> struct Context {

    StringViews lines;

    int width, height;

    Coord start;

    bool isValid(Coord coord) const {
        return coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height && getChar(coord) != '#';
    }

    bool isValid2(Coord coord) const {

        coord.x += 1000000 * width;
        coord.y += 1000000 * height;

        while (coord.x < 0) {
            coord.x += width;
        }
        while (coord.y < 0) {
            coord.y += height;
        }

        coord.x %= width;
        coord.y %= height;

        return getChar(coord) != '#';
    }

    void convert2(Coord &coord) {
        coord.x += 1000000 * width;
        coord.y += 1000000 * height;

        while (coord.x < 0) {
            coord.x += width;
        }
        while (coord.y < 0) {
            coord.y += height;
        }

        coord.x %= width;
        coord.y %= height;
    }

    inline char getChar(Coord coord) const {
        return lines[coord.y][coord.x];
    }

    void parse(auto _lines) {
        rs::copy(_lines, std::back_inserter(lines));
        width = lines[0].size();
        height = lines.size();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (getChar(Coord{x, y}) == 'S') {
                    start = {x, y};
                }
            }
        }
    }

    int compute(const Vector<Coord> &starts, const int max_steps) {
        struct Node {
            Coord position;
            int steps;
        };

        Queue<Node> q;
        Map<Coord, Map<int, bool>> visited;
        Map<Coord, bool> solution;

        for (auto start : starts) {
            q.push({start, 0});
        }

        while (!q.empty()) {

            const auto node = q.front();
            q.pop();

            auto &v = visited[node.position][node.steps];

            if (v) {
                continue;
            }

            v = true;

            if (node.steps == max_steps) {
                solution[node.position] = true;
                continue;
            }

            for (auto dir : dirs) {
                auto npos = node.position + dir;

                if (isValid(npos)) {
                    auto copy = node;
                    copy.position = npos;
                    copy.steps++;
                    q.push(copy);
                }
            }
        }

        return solution.size();
    }

    int compute2(const Vector<Coord> &starts, const int max_steps) {
        struct Node {
            Coord position;
            int steps;
        };

        Queue<Node> q;
        Map<Coord, Map<int, bool>> visited;
        Map<Coord, bool> solution;

        for (auto start : starts) {
            q.push({start, 0});
        }

        while (!q.empty()) {

            const auto node = q.front();
            q.pop();

            auto &v = visited[node.position][node.steps];

            if (v) {
                continue;
            }

            v = true;

            if (node.steps == max_steps) {
                solution[node.position] = true;
                continue;
            }

            for (auto dir : dirs) {
                auto npos = node.position + dir;

                if (isValid2(npos)) {
                    auto copy = node;
                    copy.position = npos;
                    copy.steps++;
                    q.push(copy);
                }
            }
        }

        return solution.size();
    }

    void part1() {

        auto result = compute({start}, STEPS);

        log << "Part1: " << result << endl;
    }

    void part2(auto max_steps) {

        if (max_steps < 1000000) {
            log << "Force  (" << max_steps << ") : " << compute2({start}, max_steps) << endl;
        }

        Int64 previous;
        Int64 previous_delta;

        Int64 delta_delta = 0;
        Int64 last = 0;
        Int64 last_delta = 0;

        for (int i = 0; i < 3; ++i) {
            auto steps2 = i * width + width / 2;

            auto r = compute2({start}, steps2);

            if (i != 0) {
                auto delta = r - previous;
                if (i > 1) {
                    delta_delta = delta - previous_delta;
                    last = r;
                    last_delta = delta;
                    break;
                }
                previous_delta = delta;
            }

            previous = r;
        }

        Int64 start = width * 2 + width / 2;
        Int64 result = last;
        Int64 delta = last_delta;

        while (true) {

            start += width;

            delta += delta_delta;
            result += delta;

            if (start == max_steps) {
                break;
            }
        }

        log << "Part2  (" << max_steps << ") : " << result << endl;
    }
};

template <int steps> void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
    {
        Context<steps> context;
        context.parse(lines);
        context.part1();

        if constexpr (steps == 6) {
            context.part2(6);
            context.part2(10);
            context.part2(50);
            /* context.part2(100); */
            /* context.part2(500); */
            /* context.part2(1000); */
            /* context.part2(5000); */
        } else if constexpr (steps == 7) {
            context.part2(115);
            context.part2(126);
            context.part2(137);
            context.part2(148);
            context.part2(159);
        } else {
            context.part2(26501365);
        }
    }
}

int main() {
    /* process<6>("sample.txt"); */
    process<7>("sample2.txt");
    process<64>("input.txt");

    // too low      154779446780000
    // too low      309558893560000
    // too low      315902313510000

    // incorrect
    //              315903843707200
    //              315903911283350
    //              315905373904400
    //              630905800837200
    //              630907331034400
    //              630916745267200
    //              631647207259409
    //              631640962678625
    //              631647207259409
    //              631647207275025
    //              631810909228611
    //             1263609254040000

    return 0;
}
