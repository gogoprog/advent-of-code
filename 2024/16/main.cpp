#include "../../common_fast.h"

static constexpr Array<Coord, 4> deltas = {Coord{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
enum Direction { N, E, S, W };

struct Context {
    Grid grid;
    Coord start;
    Coord end;

    void parse(auto lines) {
        grid.parse(lines);

        grid.for_each([&](auto coord, auto c) {
            if (c == 'E') {
                end = coord;
            }
            if (c == 'S') {
                start = coord;
            }

            return true;
        });
    }

    inline bool isWall(const auto &coord) const {
        return grid.get(coord) == '#';
    }

    void part1() {
        auto result{0_int64};

        struct Node {
            Coord position;
            int score;
            char dir;
        };

        Queue<Node> q;
        int best = 1000000000;

        Map<Pair<Coord, int>, int> visited;

        q.push({start, 0, E});

        auto try_dir = [&](const Node node, const Coord npos, const int dir, const int score) {
            if (!isWall(npos)) {
                auto copy = node;
                copy.score += score;
                copy.position = npos;
                copy.dir = dir;
                q.push(copy);
            }
        };

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            auto & v = visited[{node.position, node.dir}];

            if(v && v < node.score)
            {
                continue;
            }

            visited[{node.position, node.dir}] = node.score;

            if (node.position == end) {
                if (node.score < best) {
                    best = node.score;
                    log << best << endl;
                }
            }

            try_dir(node, node.position + deltas[node.dir], node.dir, 1);
            try_dir(node, node.position, (node.dir + 1) % 4, 1000);
            try_dir(node, node.position, (node.dir + 3) % 4, 1000);
        }

        result = best;

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0_int64};

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context context;
        context.parse(lines);
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
