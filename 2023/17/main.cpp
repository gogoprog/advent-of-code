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

        bool operator<(const Node &other) const {

            if (loss == other.loss) {
                auto a = position.x + position.y;
                auto b = other.position.x + other.position.y;

                return a > b;
            }
            return loss > other.loss;
        }
    };

    template <int PART> int solve() {
        PriorityQueue<Node> q;

        int best{10000000};

        Node best_node;

        Node start{{0, 0}};
        Coord goal{width - 1, height - 1};

        Map<Coord, Map<int, int>> visited;

        q.push(start);

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();

            auto &v = visited[node.position][node.lastDir * 10000 + node.steps];
            if (v == 0 || node.loss < v) {
                v = node.loss;
            } else {
                continue;
            }

            if (node.position == goal) {

                if constexpr (PART == 2) {
                    if (node.steps < 3) {
                        continue;
                    }
                }

                if (node.loss < best) {
                    best = node.loss;
                    best_node = node;
                }
            } else {
                int d = 0;
                int id = 0;
                auto ld = node.lastDir;
                for (auto &dir : dirs) {
                    d = id;
                    id++;
                    auto npos = node.position + dir;

                    if ((ld == N && d == S) || (ld == S && d == N) || (ld == E && d == W) || (ld == W && d == E)) {
                        continue;
                    }

                    if (isValid(npos)) {
                        auto copy = node;

                        if constexpr (PART == 1) {
                            if (copy.lastDir == d) {
                                copy.steps++;
                            } else {
                                copy.steps = 0;
                            }

                            if (copy.steps < 3) {
                                copy.lastDir = d;
                                copy.position = npos;
                                copy.loss += getChar(npos) - '0';

                                q.push(copy);
                            }
                        } else {

                            if (ld != -1 && copy.steps < 3) {
                                if (d != ld) {
                                    continue;
                                }
                            }

                            if (copy.lastDir == d) {
                                copy.steps++;
                            } else {
                                copy.steps = 0;
                            }

                            if (copy.steps < 10) {
                                copy.lastDir = d;
                                copy.position = npos;
                                copy.loss += getChar(npos) - '0';

                                q.push(copy);
                            }
                        }
                    }
                }
            }
        }

        return best;
    }

    void part1() {
        auto result = solve<1>();

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result = solve<2>();

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
    process("sample2.txt");
    process("input.txt");
    return 0;
}
