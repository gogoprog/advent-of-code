#include "../../common_fast.h"

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

    bool isValid(Coord coord) const {
        return coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height;
    }

    bool isValid2(Coord coord) const {
        return coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height && getChar(coord) != '#';
    }

    inline char getChar(Coord coord) const {
        return lines[coord.y][coord.x];
    }

    void parse(auto _lines) {
        rs::copy(_lines, std::back_inserter(lines));
        width = lines[0].size();
        height = lines.size();

        analyze();
    }

    struct Node {
        Coord position;
        Coord origin;
        int16_t steps{0};
        Set<Coord> visited;
    };

    template <int PART> int solve() {
        Queue<Node> q;

        int best{0};

        Node start{{1, 0}};
        Coord goal{width - 2, height - 1};

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

                            Coord result;
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

    Vector<Coord> crossroads;

    struct Road {
        Array<int, 2> points;
        int length;
        bool goal{false};
        uint8_t id{0};
        Set<Coord> path;

        bool connect(const int c) const {
            return points[0] == c || points[1] == c;
        }
    };

    Map<int, Vector<Road *>> roadmap;

    Vector<Road> roads;

    int lastCrossroadId;
    int endId;
    Road *lastRoad;

    void analyze() {
        Queue<Node> q;

        Node start{{1, 0}, {1, 0}};
        Coord goal{(width - 2), (height - 1)};

        q.push(start);

        roads.reserve(1024);

        Vector<Coord> valids;

        Map<Coord, bool> visited;

        auto get_crossroad_id = [&](Coord coord) {
            for (int i = 0; i < crossroads.size(); i++) {
                if (crossroads[i] == coord) {
                    return i;
                }
            }

            crossroads.push_back(coord);

            /* log << "crossroad " << crossroads.size() - 1 << " : " << coord << "\n"; */

            return int(crossroads.size() - 1);
        };

        get_crossroad_id({1, 0});

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            valids.resize(0);

            auto add_road = [&]() {
                for (auto &road : roads) {

                    if (road.path == node.visited) {
                        return &road;
                    }
                }

                auto road = &roads.emplace_back();
                road->length = node.steps;
                road->points[0] = get_crossroad_id(node.origin);
                road->points[1] = get_crossroad_id(node.position);
                roadmap[road->points[0]].push_back(road);
                roadmap[road->points[1]].push_back(road);
                road->id = roads.size() - 1;
                road->path = node.visited;

                /* log << "road: " << crossroads[road->points[0]] << " -> " << crossroads[road->points[1]] << " len=" <<
                 * road->length << "\n"; */
                return road;
            };

            if (node.position == goal) {
                auto r = add_road();
                r->goal = true;
                lastRoad = r;
                lastCrossroadId = get_crossroad_id(node.origin);
                endId = get_crossroad_id(node.position);
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

        log << roads.size() << " roads\n";

        for (auto kv : roadmap) {

            auto a = kv.first;
            for (auto kv2 : roadmap) {
                auto b = kv2.first;

                if (a != b) {

                    for (auto &road : roads) {
                        if (road.connect(a) && road.connect(b)) {
                            /* log << "found for " << a << " and " << b << "\n"; */
                        }
                    }
                }
            }
        }
    }

    int solve2() {
        struct Node {
            uint8_t currentPointId : 8;
            std::bitset<64> visited{0};
            int16_t length{0};
        };

        Queue<Node> q;

        auto start_node = Node{0};
        start_node.visited.set(0);

        q.push(start_node);

        auto best = 0;

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            if (node.currentPointId == endId) {
                auto len = node.length;
                if (len > best) {
                    best = len;
                }
                continue;
            }

            if (node.currentPointId == lastCrossroadId) {
                auto copy = node;
                copy.currentPointId = endId;
                copy.visited.set(endId, true);
                copy.length += lastRoad->length;

                q.push(copy);

                continue;
            }

            for (auto road : roadmap[node.currentPointId]) {

                for (auto next_p : road->points) {
                    if (!node.visited.test(next_p)) {
                        auto copy = node;
                        copy.currentPointId = next_p;
                        copy.visited.set(node.currentPointId, true);
                        copy.visited.set(next_p, true);
                        copy.length += road->length;
                        q.push(copy);
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

        if (lines.size() < 100) {

            auto r = solve<2>();
            log << "Forced2: " << r << endl;
        }

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
    process("sample2.txt");
    process("sample3.txt");
    process("input.txt");
    return 0;
}
