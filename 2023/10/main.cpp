#include "../../common_fast.h"

constexpr Array<Coord, 4> allMoves = {Coord{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

Map<char, Vector<Coord>> allowedMoves = {{'|', {{0, 1}, {0, -1}}},
                                         {'-', {{1, 0}, {-1, 0}}},
                                         {'L', {{1, 0}, {0, -1}}},
                                         {'J', {{-1, 0}, {0, -1}}},
                                         {'7', {{-1, 0}, {0, 1}}},
                                         {'F', {{1, 0}, {0, 1}}},
                                         {'.', {}},
                                         {'S', {{0, -1}, {0, 1}, {-1, 0}, {1, 0}}}};

struct Context {
    Vector<StringView> lines;
    Coord startPos;
    int width;
    int height;

    bool isValid(Coord coord) const {
        return coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height;
    }

    inline char getChar(Coord coord) const {
        return lines[coord.y][coord.x];
    }

    void parse(auto str) {
        rs::copy(str, std::back_inserter(lines));

        width = lines[0].size();
        height = lines.size();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (lines[y][x] == 'S') {
                    startPos = {x, y};
                }
            }
        }
    }

    void part1() {
        struct Node {
            int time{0};
            Coord position;
            Coord previous{-1, -1};

            bool operator<(const Node &other) const {
                return time < other.time;
            }
        };

        PriorityQueue<Node> q;
        int best = 0;
        Node bestNode;
        Node start{0, startPos};

        q.push(start);

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();

            if (node.position == startPos && node.time > 0) {
                if (node.time > best) {
                    best = node.time;
                    bestNode = node;
                }
                continue;
            }

            auto ch = getChar(node.position);

            for (auto &move : allowedMoves[ch]) {
                auto new_pos = node.position + move;

                if (!isValid(new_pos))
                    continue;

                if (node.previous != new_pos) {
                    auto copy = node;
                    copy.previous = copy.position;
                    copy.position = new_pos;
                    copy.time++;
                    q.push(copy);
                }
            }
        }

        auto result = bestNode.time / 2;

        log << "Part1: " << result << endl;
    }

    void part2() {
        struct Node {
            int time{0};
            Coord position;
            Coord previous{-1, -1};
            Vector<Coord> path;
            Vector<Coord> dirs;

            bool operator<(const Node &other) const {
                return time < other.time;
            }
        };

        PriorityQueue<Node> q;
        int best = 0;
        Node bestNode;
        Node start{0, startPos};

        q.push(start);

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();

            if (node.position == startPos && node.time > 0) {
                if (node.time > best) {
                    best = node.time;
                    bestNode = node;
                }
                continue;
            }

            auto ch = getChar(node.position);

            for (const auto &move : allowedMoves[ch]) {
                auto new_pos = node.position + move;

                if (!isValid(new_pos))
                    continue;

                if (node.previous != new_pos) {
                    auto copy = node;
                    copy.previous = copy.position;
                    copy.position = new_pos;
                    copy.path.push_back(new_pos);

                    copy.dirs.push_back(move);
                    copy.time++;
                    q.push(copy);
                }
            }
        }

        auto result = 0;
        Map<Coord, Coord> dirmap;
        Map<Coord, bool> capturedMap;

        for (int i = 0; i < bestNode.path.size(); i++) {
            dirmap[bestNode.path[i]] = bestNode.dirs[i];
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Coord pos{x, y};

                if (dirmap[pos] == Coord{0, 0}) {

                    bool captured = false;

                    for (auto delta : allMoves) {
                        bool done{false};
                        auto current = pos;

                        while (!done) {
                            current += delta;

                            auto dir = dirmap[current];

                            /* dir.x *= -1; */
                            /* dir.y *= -1; */

                            if (!isValid(current)) {
                                break;
                            } else {
                                auto ch = getChar(current);
                                if (dirmap[current] == Coord{0, 0})
                                    ch = '.';
                                switch (ch) {
                                    case '.':
                                        break;

                                    case '-':
                                        if (delta.y == dir.x) {
                                            captured = true;
                                        }

                                        done = true;
                                        break;

                                    case '|':
                                        if (delta.x == -dir.y) {
                                            captured = true;
                                        }
                                        done = true;
                                        break;

                                    case '7':
                                        if (dir.y) {
                                            if (delta.x && delta.x == -dir.y) {
                                                captured = true;
                                            }
                                            if (delta.y && delta.y == dir.y) {
                                                captured = true;
                                            }
                                        }

                                        if (dir.x) {
                                            if (delta.x && delta.x == -dir.x) {
                                                captured = true;
                                            }
                                            if (delta.y && delta.y == dir.x) {
                                                captured = true;
                                            }
                                        }

                                        done = true;
                                        break;
                                    case 'L':
                                        if (dir.y) {
                                            if (delta.x && delta.x == -dir.y) {
                                                captured = true;
                                            }
                                            if (delta.y && delta.y == dir.y) {
                                                captured = true;
                                            }
                                        }

                                        if (dir.x) {
                                            if (delta.x && delta.x == -dir.x) {
                                                captured = true;
                                            }
                                            if (delta.y && delta.y == dir.x) {
                                                captured = true;
                                            }
                                        }

                                        done = true;
                                        break;
                                    case 'F':
                                        if (dir.y) {
                                            if (delta.x && delta.x == -dir.y) {
                                                captured = true;
                                            }
                                            if (delta.y && delta.y == -dir.y) {
                                                captured = true;
                                            }
                                        }

                                        if (dir.x) {
                                            if (delta.x && delta.x == dir.x) {
                                                captured = true;
                                            }
                                            if (delta.y && delta.y == dir.x) {
                                                captured = true;
                                            }
                                        }
                                        done = true;
                                        break;
                                    case 'J':
                                        if (dir.y) {
                                            if (delta.x && delta.x == -dir.y) {
                                                captured = true;
                                            }
                                            if (delta.y && delta.y == dir.y) {
                                                captured = true;
                                            }
                                        }

                                        if (dir.x) {
                                            if (delta.x && delta.x == dir.x) {
                                                captured = true;
                                            }
                                            if (delta.y && delta.y == dir.x) {
                                                captured = true;
                                            }
                                        }

                                        done = true;
                                        break;

                                    default:
                                        done = true;
                                        break;
                                }
                            }
                        }

                        if (captured)
                            break;
                    }

                    if (captured) {
                        capturedMap[pos] = true;
                        /* log << pos << endl; */
                        result++;
                    }
                }
            }
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (capturedMap[{x, y}])
                    log << "I";
                else
                    log << lines[y][x];
            }
            log << endl;
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
    /* process("sample0.txt"); */
    /* process("sample.txt"); */
    /* process("sample2.txt"); */
    /* process("sample3.txt"); */
    /* process("sample4.txt"); */
    process("input.txt");
    // too high 4944
    // too high 2243
    return 0;
}
