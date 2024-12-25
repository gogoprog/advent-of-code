#include "../../common_fast.h"

static constexpr Array<Coord, 4> deltas = {Coord{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

struct Context {

    Grid grid;

    Set<char> plants;

    void parse(auto lines) {
        grid.parse(lines);

        grid.for_each([&](auto coord, auto ch) {
            plants.insert(ch);

            return true;
        });
    }

    Int64 process(char target) {
        Int64 result = 0;

        struct Node {
            Coord pos;
            int8_t dir;
            bool operator<(const Node &other) const {
                if (dir == other.dir) {
                    return pos < other.pos;
                }

                return dir < other.dir;
            }
        };

        Set<Coord> all_visited;

        auto compute = [&](const auto start) {
            Set<Node> visited;
            Set<Node> perimeter;
            Set<Coord> area;

            Queue<Node> q;
            q.push({start, 0});

            while (!q.empty()) {

                const auto node = q.front();
                q.pop();

                if (visited.contains(node)) {
                    continue;
                }

                visited.insert(node);
                all_visited.insert(node.pos);

                if (grid.isValid(node.pos) && grid.get(node.pos) == target) {

                    area.insert(node.pos);

                    int d = 0;
                    for (auto delta : deltas) {
                        auto copy = node;
                        copy.pos += delta;
                        copy.dir = d;

                        q.push(copy);
                        d++;
                    }

                } else {
                    perimeter.insert(node);
                }
            }

            return area.size() * perimeter.size();
        };

        grid.for_each([&](auto coord, auto ch) {
            if (ch == target) {
                if (!all_visited.contains(coord)) {
                    result += compute(coord);
                }
            }

            return true;
        });

        /* log << target << " -> " << result << endl; */

        return result;
    }

    Int64 process2(char target) {
        Int64 result = 0;

        struct Node {
            Coord pos;
            int8_t dir;
            bool operator<(const Node &other) const {
                if (dir == other.dir) {
                    return pos < other.pos;
                }

                return dir < other.dir;
            }

            bool operator==(const Node &other) const = default;
        };

        Set<Coord> all_visited;

        auto compute_sides = [](const auto perimeter) {
            Set<Node> visited;
            auto start = *perimeter.begin();
            int result = 0;
            for (auto &start : perimeter) {

                if (!visited.contains(start)) {

                    auto current = start;

                    do {

                        visited.insert(current);

                        /* log << current.pos << " | " << deltas[current.dir] << endl; */

                        // same dir
                        {
                            auto delta = deltas[(current.dir + 1) % 4];
                            auto test_node = Node{current.pos + delta, current.dir};
                            if (perimeter.contains(test_node)) {
                                current = test_node;
                                continue;
                            }
                        }

                        // right turn
                        {
                            static constexpr Array<Coord, 4> deltas = {Coord{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
                            auto test_node = Node{current.pos + deltas[current.dir], int8_t((current.dir + 1) % 4)};
                            if (perimeter.contains(test_node)) {
                                current = test_node;
                                result++;
                                continue;
                            }
                        }

                        // left turn
                        {
                            auto test_node = Node{current.pos, int8_t((current.dir + 3) % 4)};
                            if (perimeter.contains(test_node)) {
                                current = test_node;
                                result++;
                                continue;
                            }
                        }

                        log << "Impossibru" << endl;

                    } while (current != start);
                }
            }

            return result;
        };

        auto compute = [&](const auto start) {
            Set<Node> visited;
            Set<Node> perimeter;
            Set<Coord> area;

            Queue<Node> q;
            q.push({start, -1});

            while (!q.empty()) {

                const auto node = q.front();
                q.pop();

                if (visited.contains(node)) {
                    continue;
                }

                visited.insert(node);
                all_visited.insert(node.pos);

                if (grid.isValid(node.pos) && grid.get(node.pos) == target) {

                    area.insert(node.pos);
                    int d = 0;
                    for (auto delta : deltas) {
                        auto copy = node;
                        copy.pos += delta;
                        copy.dir = d;

                        q.push(copy);
                        d++;
                    }

                } else {
                    perimeter.insert(node);
                }
            }

            auto r = compute_sides(perimeter);

            /* log << target << " has " << r << " sides" << endl; */

            return r * area.size();
        };

        grid.for_each([&](auto coord, auto ch) {
            if (ch == target) {
                if (!all_visited.contains(coord)) {
                    result += compute(coord);
                }
            }

            return true;
        });

        /* log << target << " -> " << result << endl; */

        return result;
    }

    void part1(auto lines) {
        auto result{0_int64};

        for (auto ch : plants) {
            result += process(ch);
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};

        for (auto ch : plants) {
            result += process2(ch);
        }

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context context;
        context.parse(lines);
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample2.txt");
    process("sample3.txt");
    process("sample4.txt");
    process("sample.txt");
    process("input.txt");
    return 0;
}
