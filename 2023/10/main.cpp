#include "../../common_fast.h"

Map<char, Vector<Coord>> allowedMoves = {{'|', {{0, 1}, {0, -1}}},
                                         {'-', {{1, 0}, {-1, 0}}},
                                         {'L', {{1, 0}, {0, -1}}},
                                         {'J', {{-1, 0}, {0, -1}}},
                                         {'7', {{-1, 0}, {0, 1}}},
                                         {'F', {{1, 0}, {0, 1}}},
                                         {'.', {}},
                                         {'S', {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}}};

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
        Map<Coord, int> visited;
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

            bool operator<(const Node &other) const {
                return time < other.time;
            }
        };

        PriorityQueue<Node> q;
        Map<Coord, int> visited;
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

                if (node.previous != new_pos) {
                    auto copy = node;
                    copy.previous = copy.position;
                    copy.path.push_back(copy.position);
                    copy.position = new_pos;
                    copy.time++;
                    q.push(copy);
                }
            }
        }

        auto result = bestNode.time / 2;

        log << "Par21: " << result << endl;
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
    process("sample0.txt");
    process("sample.txt");
    process("sample2.txt");
    process("sample3.txt");
    process("input.txt");
    return 0;
}
