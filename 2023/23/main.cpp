#include "../../common_fast.h"

using Int = uint8_t;

struct Coord8 {
    Int x, y;

    bool operator==(const Coord8 &other) const {
        return x == other.x && y == other.y;
    }

    Coord8 &operator+=(const Coord &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    bool operator<(const Coord8 &other) const {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }
};

Coord8 operator+(const Coord8 &a, const Coord8 &b) {
    return {Int(a.x + b.x), Int(a.y + b.y)};
}

Coord8 operator+(const Coord8 &a, const Coord &b) {
    return {Int(a.x + b.x), Int(a.y + b.y)};
}

enum Directions { N, E, S, W };
constexpr Array<Coord, 4> dirs = {Coord{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

auto mapping = Map<char, int>{
    {'>', E},
    {'^', N},
    {'v', S},
    {'<', W},
};

struct Context {

    StringViews lines;

    int width, height;

    bool isValid(Coord8 coord) const {
        return coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height;
    }

    bool isValid2(Coord8 coord) const {
        return coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height && getChar(coord) != '#';
    }

    inline char getChar(Coord8 coord) const {
        return lines[coord.y][coord.x];
    }

    void parse(auto _lines) {
        rs::copy(_lines, std::back_inserter(lines));
        width = lines[0].size();
        height = lines.size();
    }

    struct Node {
        Coord8 position;
        int16_t steps{0};
        Set<Coord8> visited;
    };

    template <int PART> int solve() {
        Queue<Node> q;

        int best{0};

        Node start{{1, 0}};
        Coord8 goal{Int(width - 2), Int(height - 1)};

        /* Map<Coord, int> visited; */

        q.push(start);

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            /* auto &v = visited[node.position]; */
            /* if (v == 0 || node.steps < v) { */
            /* v = node.steps; */
            /* } else { */
            /* continue; */
            /* } */

            if (node.position == goal) {

                if (node.steps > best) {
                    best = node.steps;
                    log << "best: " << best << "\n";
                }
            }

            for (auto d = 0; d < 4; d++) {
                auto &dir = dirs[d];
                auto npos = node.position + dir;

                if (isValid(npos)) {

                    if (!node.visited.contains(npos)) {

                        auto ch = getChar(npos);

                        auto proceed_next_max = [&]() {
                            auto copy = node;

                            copy.position = npos;
                            copy.visited.insert(copy.position);
                            copy.steps++;

                            bool first = true;
                            auto last = copy.position;

                            int last_dir = d;

                            auto has_only_one_next = [&](auto &next) {
                                int r = 0;

                                int ld = last_dir;

                                for (auto d2 = 0; d2 < 4; d2++) {

                                    if (((d2 + 2) % 4) != ld) {

                                        auto other = npos + dirs[d2];

                                        if (isValid2(other)) {
                                            next = other;
                                            last_dir = d2;
                                            r++;
                                        }
                                    }
                                }

                                return r == 1;
                            };

                            Coord8 result;
                            while (has_only_one_next(result)) {
                                copy.steps++;

                                last = copy.position;

                                copy.position = result;

                                npos = result;

                                if (first) {
                                    copy.visited.insert(copy.position);
                                    first = false;
                                }
                                /* break; */
                            }

                            copy.visited.insert(last);
                            copy.visited.insert(copy.position);

                            q.push(copy);
                        };

                        if constexpr (PART == 1) {
                            switch (ch) {
                                case '#':
                                    break;
                                case '.': {
                                    proceed_next_max();
                                } break;
                                case '>':
                                case '<':
                                case '^':
                                case 'v':

                                    if (d == mapping[ch]) {
                                        auto copy = node;
                                        copy.position = npos;
                                        copy.steps++;
                                        copy.visited.insert(npos);
                                        q.push(copy);
                                    }
                                    break;
                            }
                        } else {

                            switch (ch) {
                                case '#':
                                    break;
                                default:
                                    proceed_next_max();
                                    break;
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
    process("input.txt");
    return 0;
}
