#include "../../common_fast.h"

enum Directions { N, E, S, W };
constexpr Array<Coord, 4> dirs = {Coord{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

struct Context {

    StringViews lines;

    int width, height;

    bool isValid(Coord coord) const {
        return coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height;
    }

    inline char getChar(Coord coord) const {
        return lines[coord.y][coord.x];
    }

    void parse(auto _lines) {
        rs::copy(_lines, std::back_inserter(lines));
        width = lines[0].size();
        height = lines.size();
    }

    struct Node {
        Coord position;
        int steps{0};
        int lastDir{-1};
        int loss{0};

        Vector<Coord> path;

        bool operator<(const Node &other) const {
            if (position.x == other.position.x) {
                return position.y < other.position.y;
            }
            return position.x < other.position.x;
        }
    };

    int solve() {
        Queue<Node> q;

        int best{1000000};

        Node best_node;

        Node start{{0, 0}};
        Coord goal{width - 1, height - 1};

        Map<Coord, int> visited;

        q.push(start);

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            /* if (node.loss > best) */
            /* continue; */

            auto &v = visited[node.position];
            if (v == 0 || node.loss <= v) {
                v = node.loss;
            } else {
                continue;
            }

            if (node.position == goal) {
                if (node.loss < best) {
                    best = node.loss;
                    best_node = node;
                    log << "yup: " << node.loss << endl;
                }
            } else {
                int d = 0;
                for (auto &dir : dirs) {
                    auto npos = node.position + dir;

                    if (isValid(npos)) {
                        auto copy = node;

                        if (copy.lastDir == d) {
                            copy.steps++;
                        } else {
                            copy.steps = 0;
                        }

                        if (copy.steps < 3) {
                            copy.lastDir = d;
                            copy.position = npos;
                            copy.loss += getChar(npos) - '0';

                            copy.path.push_back(npos);

                            q.push(copy);
                        }
                    }

                    d++;
                }
            }
        }

        log << best_node.path << endl;

        return best;
    }

    void part1() {
        auto result = solve();

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0};

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
    {
        Context context;
        context.parse(lines);
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample.txt");
    /* process("input.txt"); */
    return 0;
}
