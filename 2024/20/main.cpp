#include "../../common_fast.h"

static constexpr Array<Coord, 4> deltas = {Coord{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

template <int p1, int p2> struct Context {
    Grid grid;
    Coord start;
    Coord end;
    Map<Coord, int> noCheatVisited;
    Map<Coord, Map<Coord, int>> times;

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

    inline bool isValid(const Coord &pos) const {
        return grid.get(pos) != '#';
    }

    int computeTime(const Coord _start, const Coord _end) {
        int best{std::numeric_limits<int>::max()};

        struct Node {
            Coord pos;
            int time;
            inline bool operator<(const Node &other) const {
                return time > other.time;
            }
        };

        PriorityQueue<Node> q;
        q.push({_start, 0});

        Map<Coord, int> visited;

        while (!q.empty()) {
            auto node = q.top();
            q.pop();

            auto &v = visited[node.pos];

            if (v && v <= node.time) {
                continue;
            }

            v = node.time;

            auto tte = times[_end][node.pos];

            if (tte && tte != std::numeric_limits<int>::max()) {
                if (node.time + tte < best) {
                    best = node.time + tte;
                }
            } else if (node.pos == _end) {
                if (node.time < best) {
                    best = node.time;
                }
            } else {

                for (auto delta : deltas) {
                    auto npos = node.pos + delta;

                    if (grid.isValid(npos)) {
                        if (isValid(npos)) {
                            auto copy = node;
                            copy.pos = npos;
                            copy.time++;
                            q.push(copy);
                        }
                    }
                }
            }
        }

        return best;
    }

    template <int cheats> int computeCheats(const int mindiff) {
        int result = 0;
        int base = computeTime(start, end);

        grid.for_each([&](auto pos, auto c) {
            if (isValid(pos)) {
                times[end][pos] = computeTime(pos, end);
                times[start][pos] = computeTime(pos, start);
            }

            return true;
        });

        for (auto kv1 : times[start]) {
            for (auto kv2 : times[end]) {

                if (kv1.first != kv2.first) {
                    auto delta = kv2.first - kv1.first;
                    auto man = delta.manhattan();

                    if (man <= cheats) {
                        if (kv1.second + kv2.second + man <= base - mindiff) {
                            result++;
                        }
                    }
                }
            }
        }

        return result;
    }

    void part1() {
        auto result = computeCheats<2>(p1);

        log << "Part1: " << result << endl;
    }

    void part2() {
        times.clear();
        noCheatVisited.clear();

        auto result = computeCheats<20>(p2);

        log << "Part2: " << result << endl;
    }
};

template <int p1, int p2> void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context<p1, p2> context;
        context.parse(lines);
        context.part1();
        context.part2();
    }
}

int main() {
    process<20, 72>("sample.txt");
    process<100, 100>("input.txt");

    return 0;
}
