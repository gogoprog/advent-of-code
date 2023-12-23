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

Logger &operator<<(Logger &logger, Coord8 c) {
    printf("(%d, %d)", c.x, c.y);
    return logger;
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

        analyze();
    }

    struct Node {
        Coord8 position;
        Coord8 origin;
        int16_t steps{0};
        Set<Coord8> visited;
    };

    template <int PART> int solve() {
        Queue<Node> q;

        int best{0};

        Node start{{1, 0}};
        Coord8 goal{Int(width - 2), Int(height - 1)};

        q.push(start);

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            if (node.position == goal) {

                if (node.steps > best) {
                    best = node.steps;
                    /* log << "best: " << best << "\n"; */
                    /* log << node.visited.size() << endl; */
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

    struct Road {
        Array<Coord8, 2> points;
        int length;
        bool goal{false};
    };

    Vector<Road> roads;

    Map<Coord8, Vector<Pair<Road *, int>>> roadmap;

    void analyze() {
        Queue<Node> q;

        Node start{{1, 0}, {1, 0}};
        Coord8 goal{Int(width - 2), Int(height - 1)};

        q.push(start);

        roads.reserve(1024);

        Vector<Coord8> valids;

        Map<Coord8, bool> visited;

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            valids.resize(0);

            auto add_road = [&]() {
                for (auto pair : roadmap[node.position]) {
                    auto road = pair.first;
                    auto p = pair.second;

                    if (road->points[p - 1] == node.origin) {
                        return road;
                    }
                }
                for (auto pair : roadmap[node.origin]) {
                    auto road = pair.first;
                    auto p = pair.second;

                    if (road->points[p - 1] == node.position) {
                        return road;
                    }
                }

                auto road = &roads.emplace_back();
                road->length = node.steps + 1;
                road->points[0] = node.origin;
                road->points[1] = node.position;
                roadmap[road->points[0]].push_back({road, 0});
                roadmap[road->points[1]].push_back({road, 1});
                /* log << "road: " << road->points[0] << " -> " << road->points[1] << "\n"; */
                return road;
            };

            if (node.position == goal) {
                auto r = add_road();
                r->goal = true;
            }

            for (const auto &dir : dirs) {
                auto npos = node.position + dir;

                if (isValid2(npos)) {
                    valids.push_back(npos);
                }
            }

            if (valids.size() <= 2) {

                for (auto &valid : valids) {
                    if (!node.visited.contains(valid)) {
                        auto copy = node;
                        copy.position = valid;
                        copy.steps++;
                        copy.visited.insert(valid);
                        q.push(copy);
                    }
                }
            } else {
                add_road();

                /* log << "cross detected! " << node.position << endl; */
                for (auto &valid : valids) {
                    auto &v = visited[valid];
                    if (v) {
                        continue;
                    }
                    v = true;
                    if (!node.visited.contains(valid)) {
                        auto copy = Node{valid, node.position};
                        copy.visited.insert(valid);
                        copy.visited.insert(node.position);
                        copy.steps++;
                        q.push(copy);
                    }
                }
            }
        }

        /* for (Int y = 0; y < height; y++) { */
        /*     for (Int x = 0; x < width; x++) { */
        /*         if (getChar(Coord8{x, y}) == '#') { */
        /*             log << '#'; */
        /*         } else if (roadmap.contains(Coord8{x, y})) { */
        /*             log << 'X'; */
        /*         } else { */
        /*             log << '.'; */
        /*         } */
        /*     } */

        /*     log << endl; */
        /* } */
    }

    int solve2() {

        struct Node {
            Road *currentRoad;
            Int currentPoint;
            Set<Road *> visited;
            Int length{0};
        };

        Queue<Node> q;

        auto start_node = Node{&roads.front(), 1, {&roads.front()}, Int(roads[0].length - 1)};

        q.push(start_node);

        auto best = 0;

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            const auto position = node.currentRoad->points[node.currentPoint];

            if (node.currentRoad->goal) {
                /* log << "found" << endl; */
                best = node.length;
                continue;
            }

            for (auto pair : roadmap[position]) {
                if (!node.visited.contains(pair.first)) {
                    auto copy = node;
                    copy.currentRoad = pair.first;
                    copy.currentPoint = 1 - pair.second;
                    copy.length += pair.first->length;
                    copy.visited.insert(pair.first);
                    q.push(copy);
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
        /* auto result = solve<2>(); */

        auto result = solve2();

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
