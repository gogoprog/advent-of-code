#include "../../common.h"

enum { N, S, W, E };

struct Blizzard {
    Coord start;
    char c;

    bool collides(const Coord pos, const int time, const int w, const int h) const {
        auto bpos = start;

        switch (c) {
            case '>':
                /* bpos.x += (time % w); */
                bpos.x += time;
                bpos.x %= w;

                break;
            case '<':
                /* bpos.x -= (time % w); */
                bpos.x -= time;

                while (bpos.x < 0) {
                    bpos.x += w;
                }
                break;
            case 'v':
                /* bpos.y += (time % h); */
                bpos.y += time;

                bpos.y %= h;

                break;
            case '^':
                /* bpos.y -= (time % h); */
                bpos.y -= time;

                while (bpos.y < 0) {
                    bpos.y += h;
                }
                break;

            default:
                assert(false);
        }

        return bpos == pos;
    }
};

struct Context {
    Strings data;
    Vector<Blizzard> blizzards;

    int width, height;

    Coord startPosition;
    Coord endPosition;

    void parse(const Strings &lines) {
        width = lines[0].size() - 2;
        height = lines.size() - 2;

        for (int y = 1; y < lines.size() - 1; y++) {
            for (int x = 1; x < lines[0].size() - 1; x++) {
                if (lines[y][x] != '.') {
                    blizzards.push_back({});
                    auto &blizzard = blizzards.back();
                    blizzard.start = {x - 1, y - 1};
                    blizzard.c = lines[y][x];
                }
            }
        }

        endPosition = {width - 1, height};
        startPosition = {0, -1};
    }

    bool isValid(const Coord pos, const int time) const {
        if (pos == endPosition || pos == startPosition) {
            return true;
        }

        if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height)
            return false;

        for (auto &b : blizzards) {
            if (b.collides(pos, time, width, height)) {
                return false;
            }
        }
        return true;
    }

    void print(const int time, const Coord ppos) {
        for (int x = 0; x < width + 2; x++) {
            log << '#';
        }
        log << endl;
        for (int y = 0; y < height; y++) {

            log << '#';
            for (int x = 0; x < width; x++) {
                auto pos = Coord{x, y};

                if (ppos == pos) {
                    log << 'E';
                } else {
                    char c = 0;

                    for (auto &b : blizzards) {
                        if (b.collides(pos, time, width, height)) {
                            if (c) {
                                c = 'X';
                            } else {
                                c = b.c;
                            }
                        }
                    }

                    if (c) {
                        log << c;
                    } else {
                        log << ".";
                    }
                }
            }

            log << '#';
            log << endl;
        }
        for (int x = 0; x < width + 2; x++) {
            log << '#';
        }
        log << endl;
    }

    int compute(Coord start_pos, Coord end_pos, const int start_time) {
        struct Node {
            int time{0};
            Coord position;
            int distance{10000000};

            bool operator<(const Node &other) const {
                if (distance != other.distance) {
                    return distance > other.distance;
                }
                return time < other.time;
            }
        };

        PriorityQueue<Node> q;

        Node start{start_time, start_pos};
        q.push(start);

        int best{5000000};

        Node best_node;

        Map<Pair<Coord, int>, bool> visited;

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();

            if (node.position == end_pos) {
                if (node.time < best) {
                    best = node.time;
                    best_node = node;
                }
            }

            if (visited[{node.position, node.time}]) {
                continue;
            }

            visited[{node.position, node.time}] = true;

            if (node.time >= best) {
                continue;
            }

            constexpr Coord deltas[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

            for (auto delta : deltas) {
                auto npos = node.position + delta;

                if (isValid(npos, node.time + 1)) {
                    auto copy = node;
                    copy.time = node.time + 1;
                    copy.position = npos;
                    copy.distance = (end_pos - npos).manhattan();
                    q.push(copy);
                }
            }

            {
                auto copy = node;
                if (isValid(node.position, node.time + 1)) {
                    copy.distance = (end_pos - node.position).manhattan();
                    copy.time++;
                    q.push(copy);
                }
            }
        }

        return best - start_time;
    }

    void part1() {
        auto result{compute(startPosition, endPosition, 0)};

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0};

        result += compute(startPosition, endPosition, 0);
        result += compute(endPosition, startPosition, result);
        result += compute(startPosition, endPosition, result);

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
