#include "../../common_fast.h"

static constexpr Array<Coord, 4> deltas = {Coord{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

template <int p1, int p2> struct Context {
    Grid grid;
    Coord start;
    Coord end;
    Map<Coord, int> noCheatVisited;

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

    int computeNoCheat() {
        int best{std::numeric_limits<int>::max()};

        struct Node {
            Coord pos;
            int time;
            inline bool operator<(const Node &other) const {
                return time > other.time;
            }
        };

        PriorityQueue<Node> q;
        q.push({start, 0});

        auto &visited = noCheatVisited;

        while (!q.empty()) {
            auto node = q.top();
            q.pop();

            auto &v = visited[node.pos];

            if (v && v < node.time) {
                continue;
            }

            v = node.time;

            if (node.pos == end) {
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
        int base = computeNoCheat();

        log << "No cheat: " << base << endl;

        struct Node {
            Coord pos;
            int time;
            bool cheating;
            int cheatLeft;
            Coord cheatPos1;
            Coord cheatPos2;

            inline bool canCheat() const {
                return cheatLeft > 0;
            }

            inline bool operator<(const Node &other) const {
                return time > other.time;
            }
        };

        PriorityQueue<Node> q;
        q.push({start, 0, false, cheats - 1, {0, 0}});

        Map<Pair<Coord, Coord>, int> visited;

        Set<Pair<Coord, Coord>> solutions;

        while (!q.empty()) {
            auto node = q.top();
            q.pop();

            {
                auto &v = noCheatVisited[node.pos];

                if (v && v < node.time - mindiff) {
                    continue;
                }
            }

            auto &v = visited[{node.pos, node.cheatPos1}];

            if (v && v <= node.time) {
                continue;
            }

            if (solutions.contains({node.cheatPos1, node.cheatPos2})) {
                continue;
            }

            v = node.time;

            if (node.pos == end) {
                if (node.time <= base - mindiff) {

                    if (!solutions.contains({node.cheatPos1, node.cheatPos2})) {
                        log << "Cheated at : " << node.cheatPos1 << " - " << node.cheatPos2 << " | ";
                        log << "Saved : " << base - node.time << endl;
                        solutions.insert({node.cheatPos1, node.cheatPos2});
                    }
                }
            } else {

                if (node.cheating && node.cheatLeft > 0) {
                    node.cheatLeft--;
                }

                for (auto delta : deltas) {
                    auto npos = node.pos + delta;

                    if (grid.isValid(npos)) {
                        if (isValid(npos)) {
                            auto copy = node;
                            copy.pos = npos;
                            copy.time++;
                            q.push(copy);
                        } else {
                            if (node.canCheat()) {
                                auto copy = node;
                                if (!copy.cheating) {
                                    copy.cheatPos1 = npos;
                                    copy.cheating = true;
                                }
                                copy.cheatPos2 = npos;
                                copy.pos = npos;
                                copy.time++;
                                q.push(copy);
                            }
                        }
                    }
                }
            }
        }

        result = solutions.size();

        return result;
    }

    void part1() {
        auto result = computeCheats<2>(p1);

        log << "Part1: " << result << endl;
    }

    void part2() {
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
    process<20, 20>("sample.txt");
    /* process<100, 100>("input.txt"); */
    return 0;
}
