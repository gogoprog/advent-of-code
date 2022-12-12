#include "../../common.h"

struct Context {
    Vector<String> data;

    Coord start;
    Coord end;
    Coord dimensions;

    void init(const Vector<String> &lines) {
        data = lines;

        dimensions.x = lines[0].size();
        dimensions.y = lines.size();

        for (int y = 0; y < dimensions.y; y++) {
            for (int x = 0; x < dimensions.x; x++) {

                if (data[y][x] == 'S') {
                    start = {x, y};
                } else if (data[y][x] == 'E') {
                    end = {x, y};
                }
            }
        }
    }

    bool isValid(const Coord p) {
        return p.x >= 0 && p.y >= 0 && p.x < dimensions.x && p.y < dimensions.y;
    }

    char get(const Coord p) const {

        if (p == start)
            return 'a';
        if (p == end)
            return 'z';
        return data[p.y][p.x];
    }

    struct Node {
        Coord current;
        Vector<Coord> path;
        bool operator<(const Node &other) const {
            return path.size() > other.path.size();
        }
    };

    int process(const Coord start) {
        constexpr Vector2 directions[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

        PriorityQueue<Node> q;
        q.push({start, {}});

        Map<Coord, bool> visited;

        int best{10000000};

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();

            if (visited[node.current]) {
                continue;
            } else {
                visited[node.current] = true;
            }

            if (node.current == end && node.path.size() < best) {
                best = node.path.size();
                continue;
            }

            auto current_value = get(node.current);

            for (auto dir : directions) {
                auto new_pos = node.current + dir;

                if (isValid(new_pos)) {

                    auto new_value = get(new_pos);

                    if (new_value - current_value <= 1) {

                        if (std::find(node.path.begin(), node.path.end(), new_pos) == node.path.end()) {
                            auto copy = node;
                            copy.path.push_back(new_pos);
                            copy.current = new_pos;
                            q.push(copy);
                        }
                    }
                }
            }
        }

        return best;
    }

    void part1() {
        auto best = process(start);
        log << "Part1: " << best << endl;
    }

    void part2() {
        int best = 100000000;
        for (int y = 0; y < dimensions.y; y++) {
            for (int x = 0; x < dimensions.x; x++) {
                auto pos = Coord{x, y};
                if (get(pos) == 'a') {
                    auto r = process(pos);
                    if (r < best) {
                        best = r;
                    }
                }
            }
        }
        log << "Part1: " << best << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx;

    ctx.init(lines);
    ctx.part1();
    ctx.part2();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
