#include "../../common_fast.h"

static constexpr Array<Coord, 4> deltas = {Coord{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

template <int Size, int P1Size> struct Context {

    Vector<Coord> bytes;

    Map<Coord, bool> fallen;

    Coord start;
    Coord end;

    bool isValid(const Coord coord) const {

        if (coord.x >= 0 && coord.x < Size && coord.y >= 0 && coord.y < Size) {
            return fallen.find(coord) == fallen.end();
        }
        return false;
    }

    void parse(auto lines) {
        auto to_coord = [](auto line) {
            Coord coord;
            auto v = line | rv::split_sv(',');
            coord.x = v | rv::get0 | rv::to_int;
            coord.y = v | rv::get1 | rv::to_int;
            return coord;
        };

        auto v = lines | rv::transform(to_coord);

        rs::copy(v, std::back_inserter(bytes));
        start = {0, 0};
        end = {Size - 1, Size - 1};
    }

    Int64 attempt() {
        auto result{0_int64};

        struct Node {
            Coord pos;
            int steps;

            int left = 0;

            void computeLeft(const Coord target) {
                left = (target - pos).manhattan();
            }

            bool operator<(const Node &other) const {
                if (left == other.left) {
                    return steps < other.steps;
                }
                return left > other.left;
            }
        };

        PriorityQueue<Node> q;
        Map<Coord, int> visited;
        int best{10000000};

        q.push({start, 0});

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();

            auto &v = visited[node.pos];

            if (v && v <= node.steps || node.steps >= best) {
                continue;
            }

            v = node.steps;

            if (node.pos == end) {
                if (node.steps < best) {
                    best = node.steps;
                    /* log << best << endl; */
                }
            } else {
                for (auto delta : deltas) {
                    auto npos = node.pos + delta;
                    if (isValid(npos)) {
                        auto copy = node;
                        copy.pos = npos;
                        copy.steps++;
                        copy.computeLeft(end);
                        q.push(copy);
                    }
                }
            }
        }

        result = best;

        return result;
    }

    void part1() {
        auto result{0_int64};

        for (int i = 0; i < P1Size; ++i) {
            fallen[bytes[i]] = true;
        }

        result = attempt();

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0_int64};

        for (int i = P1Size; i < bytes.size(); ++i) {
            fallen[bytes[i]] = true;

            result = attempt();

            if (result == 10000000) {

                log << "Part2: " << bytes[i] << endl;
                break;
            }
        }
    }
};

template <int N, int P1> void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context<N, P1> context;
        context.parse(lines);
        context.part1();
        context.part2();
    }
}

int main() {
    process<7, 12>("sample.txt");
    process<71, 1024>("input.txt");
    return 0;
}
