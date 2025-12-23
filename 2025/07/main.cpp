#include "../../common_fast.h"

struct Context {
    Grid grid;

    void parse(auto lines) {
        grid.parse(lines);
    }

    void part1() {
        auto result{0_int64};

        Map<Coord, bool> visited;
        Queue<Coord> q;

        q.push({grid.width / 2, 0});

        while (!q.empty()) {
            auto pos = q.front();
            q.pop();

            if (visited[pos]) {
                continue;
            }

            visited[pos] = true;

            ++pos.y;

            if (pos.y < grid.height) {
                auto ch = grid.get(pos);

                switch (ch) {
                    case '.':
                        q.push(pos);
                        continue;

                    case '^':
                        auto copy = pos;
                        copy.x--;
                        q.push(copy);
                        copy.x++;
                        copy.x++;
                        q.push(copy);
                        result++;
                        break;
                }
            }
        }

        log << "Part1: " << result << endl;
    }

    Uint64 count(const Coord start, const Coord end) {
        Uint64 result{0};

        static Map<Pair<Coord, Coord>, Uint64> cache;

        using Node = Coord;

        Queue<Node> q;

        q.push({start});

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            auto pos = node;

            if (pos == end) {
                result++;
                // log << "end fond" << end;
                continue;
            }

            // log << pos << endl;

            auto c = cache.find({pos, end});
            if (c != cache.end()) {
                result += c->second;
                // log << "cache used for " << pos << endl;
                pos = end;
            } else {
                pos.y++;

                if (pos.y < grid.height) {
                    auto ch = grid.get(pos);

                    switch (ch) {
                        case '.': {
                            auto copy = node;
                            // copy.push_back({pos.x, pos.y++});
                            copy = {pos.x, pos.y};
                            q.push(copy);
                        } break;

                        case '^': {
                            auto copy = node;
                            // copy.push_back({pos.x - 1, pos.y++});
                            copy = {pos.x - 1, pos.y};
                            q.push(copy);
                            auto copy2 = node;
                            // copy2.push_back({pos.x + 1, pos.y++});
                            copy2 = {pos.x + 1, pos.y};
                            q.push(copy2);
                        } break;
                    }
                }
            }
        }

        cache[{start, end}] = result;
        // log << "adding cache " << start << end << " = " << result << endl;

        return result;
    }

    void part2() {
        auto result{0_int64};

        auto start = Coord{Uint8(grid.width / 2), 0};

        for (int y = grid.height - 2; y > 0; --y) {

            for (int x = 1; x < grid.width - 1; ++x) {
                auto pos = Coord{x, y};

                if (grid.get({x, y - 1}) != '^' && (y & 1)) {

                    // log << pos << endl;
                    for (int i = 0; i < grid.width; ++i) {
                        Coord end{i, grid.height - 1};

                        if (grid.get({i, grid.height - 2}) != '^') {

                            auto r = count(pos, end);
                        }
                    }
                }
            }
        }

        for (int i = 0; i < grid.width; ++i) {
            Coord pos{i, grid.height - 1};
            if (grid.get({i, grid.height - 2}) != '^') {

                // log << start << " -> " << pos << endl;

                result += count(start, pos);
            }
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
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
