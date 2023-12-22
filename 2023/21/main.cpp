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

    void part1() {

        auto result = compute({start}, STEPS);

        log << "Part1: " << result << endl;
    }

    int computeMinSteps(const Coord &start, const Coord &end) {
        struct Node {
            Coord position;
            int steps;
        };

        Map<Coord, bool> visited;
        Queue<Node> q;

        q.push({start, 0});

        while (!q.empty()) {

            const auto node = q.front();
            q.pop();

            auto &v = visited[node.position];

            if (v) {
                continue;
            }

            v = true;

            if (node.position == end) {
                return node.steps;
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

        return -1;
    }

    int64_t part2(auto max_steps) {

        int64_t reachable = 0;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                auto coord = Coord{x, y};
                if (getChar(coord) != '#') {

                    auto delta = coord - start;
                    if ((delta.manhattan() % 2) != 0) {
                        reachable++;
                    }
                }
            }
        }

        log << width << 'x' << height << ':' << reachable << endl;

        int64_t size = int(max_steps/ width);

        log << "size= " << size << endl;

        auto left = max_steps % size;

        Point east{0, start.y};
        Point west{width - 1, start.y};
        Point north{start.x, 0};
        Point south{start.x, height - 1};

        int64_t reachables_from_ne = compute(Vector<Coord>{north, east}, left);
        int64_t reachables_from_nw = compute({north, west}, left);
        int64_t reachables_from_se = compute({south, east}, left);
        int64_t reachables_from_sw = compute({south, west}, left);

        log << "from ne " << reachables_from_ne << endl;
        log << "from nw " << reachables_from_nw << endl;
        log << "from se " << reachables_from_se << endl;
        log << "from sw " << reachables_from_sw << endl;

        int64_t full = 2 * size * size + 2 * size + 1;
        int64_t result = full * reachable;

        result += compute({north}, left);
        result += compute({south}, left);
        result += compute({east}, left);
        result += compute({west}, left);
        result += (size - 2) * reachables_from_ne;
        result += (size - 2) * reachables_from_se;
        result += (size - 2) * reachables_from_nw;
        result += (size - 2) * reachables_from_sw;


        /* int steps = computeMinSteps(start, {0, 0}); */
        /* log << "steps = " << steps << '\n'; */

        /* result = r; */
        /* result += visited.size() * 4 * (ww - 1); */

        /* result = compute(start, max_steps); */

        log << "Part2 (" << max_steps << ") : " << result << endl;

        return result;
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
        } else {
            context.part2(26501365);
        }
    }
}

int main() {
    /* process<6>("sample.txt"); */
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
    //              631810909228611
    //             1263609254040000

    // to test
    //              630907439467200
    return 0;
}
