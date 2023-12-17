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
        int direction;

        bool operator<(const Node &other) const {
            if (position.x == other.position.x) {
                if (position.y == other.position.y) {
                    return direction < other.direction;
                }
                return position.y < other.position.y;
            }
            return position.x < other.position.x;
        }
    };

    int solve(Node start) {
        auto result{0};

        Map<Coord, bool> energized;
        Map<Node, bool> visited;

        Queue<Node> q;

        q.push(start);

        while (!q.empty()) {
            auto node = q.front();
            q.pop();

            bool cont = true;
            while (cont) {

                if (visited[node]) {
                    break;
                }

                energized[node.position] = true;
                visited[node] = true;

                auto next_pos = node.position + dirs[node.direction];
                auto dir = node.direction;

                if (!isValid(next_pos)) {
                    break;
                } else {
                    node.position = next_pos;
                    auto ch = getChar(next_pos);

                    switch (ch) {
                        case '-':
                            if (dir == N || dir == S) {

                                auto split1 = node;
                                split1.direction = E;
                                q.push(split1);
                                auto split2 = node;
                                split2.direction = W;
                                q.push(split2);

                                cont = false;
                            } else {
                                node.position = next_pos;
                            }
                            break;
                        case '|':
                            if (dir == W || dir == E) {
                                auto split1 = node;
                                split1.direction = N;
                                q.push(split1);
                                auto split2 = node;
                                split2.direction = S;
                                q.push(split2);

                                cont = false;
                            } else {
                                node.position = next_pos;
                            }
                            break;
                        case '\\':
                            switch (dir) {
                                case N:
                                    dir = W;
                                    break;
                                case S:
                                    dir = E;
                                    break;
                                case W:
                                    dir = N;
                                    break;
                                case E:
                                    dir = S;
                                    break;
                            }
                            node.direction = dir;
                            break;
                        case '/':
                            switch (dir) {
                                case N:
                                    dir = E;
                                    break;
                                case S:
                                    dir = W;
                                    break;
                                case W:
                                    dir = S;
                                    break;
                                case E:
                                    dir = N;
                                    break;
                            }
                            node.direction = dir;
                            break;
                    }
                }
            }
        }

        result = energized.size();

        return result;
    }

    void part1() {
        auto result = solve({{0, 0}, E});

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0};

        for (int x = 0; x < width; x++) {
            result = std::max(result, solve({{x, 0}, S}));
            result = std::max(result, solve({{x, height - 1}, N}));
        }

        for (int y = 0; y < height; y++) {
            result = std::max(result, solve({{0, y}, E}));
            result = std::max(result, solve({{width - 1, y}, W}));
        }

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
    process("input.txt");
    return 0;
}
