#include "../../common.h"
#include <limits>

template <int wrap_count> struct Context {
    static constexpr Array<Point, 4> directions = {Point{0, -1}, Point{1, 0}, Point{0, 1}, Point{-1, 0}};
    Vector<String> data;
    int size;

    void init() {
        size = data.size();
    }

    bool exists(const Point &p) const {
        auto x = p.x;
        auto y = p.y;
        return x >= 0 && x < size * wrap_count && y >= 0 && y < size * wrap_count;
    }

    int get(const Point &p) const {

        auto x = p.x % size;
        auto y = p.y % size;
        auto value = data[y][x] - '0';
        auto inc = int(p.x / size) + int(p.y / size);

        value += inc;

        while (value > 9) {
            value -= 9;
        }

        return value;
    }

    bool isEnd(const Point &p) const {
        return p.x == size * wrap_count - 1 && p.y == size * wrap_count - 1;
    }

    struct Node {
        Point last;
        int risk{0};

        bool operator<(const Node &other) const {
            auto d = distance();
            auto other_d = other.distance();

            if (d == other_d) {
                return risk > other.risk;
            }

            return d > other_d;
        }

        int distance() const {
            return last.x + last.y;
        }
    };

    void run() {
        PriorityQueue<Node> q;

        q.push({{{0, 0}}});

        Node best{{}, std::numeric_limits<int>::max()};

        Map<Point, int> bestPerPos;

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();
            const auto last = node.last;

            if (node.risk > best.risk) {
                continue;
            }

            if (bestPerPos.find(last) == bestPerPos.end() || node.risk < bestPerPos[last]) {
                bestPerPos[last] = node.risk;

                if (!isEnd(last)) {
                    for (auto &dir : directions) {
                        auto next = last + dir;

                        if (exists(next)) {
                            auto copy = node;
                            copy.last = next;
                            copy.risk += get(next);

                            q.push(copy);
                        }
                    }

                } else {
                    if (node.risk < best.risk) {
                        best = node;
                    }
                }
            }
        }

        log << "Part" << wrap_count << ": " << best.risk << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    {
        Context<1> ctx{lines};
        ctx.init();
        ctx.run();
    }
    {
        Context<5> ctx{lines};
        ctx.init();
        ctx.run();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
