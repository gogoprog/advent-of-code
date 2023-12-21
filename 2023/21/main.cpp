#include "../../common_fast.h"

constexpr Array<Coord, 4> dirs = {Coord{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

template <int STEPS> struct Context {

    StringViews lines;

    int width, height;

    Coord start;

    bool isValid(Coord coord) const {
        return coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height && getChar(coord) != '#';
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

    void part1() {
        auto result{0};

        struct Node {
            Coord position;
            int steps;
        };

        Queue<Node> q;

        Map<Coord, Map<int, bool>> visited;

        Map<Coord, bool> solution;

        q.push({start, 0});

        while (!q.empty()) {

            const auto node = q.front();
            q.pop();

            auto &v = visited[node.position][node.steps];

            if (v) {
                continue;
            }

            v = true;

            if (node.steps == STEPS) {
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

        result = solution.size();

        log << "Part1: " << result << endl;
    }

    void part2(auto max_steps) {
        auto result{0};

        log << "Part2 (" << max_steps << ") : " << result << endl;
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
            context.part2(100);
            context.part2(500);
            context.part2(1000);
            context.part2(5000);
        }
    }
}

int main() {
    process<6>("sample.txt");
    process<64>("input.txt");
    return 0;
}
