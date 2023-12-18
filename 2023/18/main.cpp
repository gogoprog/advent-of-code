#include "../../common_fast.h"

enum Directions { U, R, D, L };
constexpr Array<Coord, 4> dirs = {Coord{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

struct Dig {
    int direction;
    int meters;

    int direction2;
    int meters2;
};

template <> struct std::hash<Coord> {
    std::size_t operator()(const Coord &k) const {
        return k.x + 10000000 * k.y;
    }
};

struct Context {
    Vector<Dig> digs;

    void parse(auto lines) {
        auto result{0};

        for (auto line : lines) {

            auto &dig = digs.emplace_back();

            auto parts = line | rv::split_sv(' ');
            auto dirsv = parts | rv::get0;
            auto meters = parts | rv::get1 | rv::to_int;
            auto colorsv = parts | rv::get2;

            switch (dirsv[0]) {
                case 'U':
                    dig.direction = U;
                    break;
                case 'D':
                    dig.direction = D;
                    break;
                case 'L':
                    dig.direction = L;
                    break;
                case 'R':
                    dig.direction = R;
                    break;
            }

            dig.meters = meters;

            switch (colorsv[7]) {
                case '3':
                    dig.direction2 = U;
                    break;
                case '1':
                    dig.direction2 = D;
                    break;
                case '2':
                    dig.direction2 = L;
                    break;
                case '0':
                    dig.direction2 = R;
                    break;
            }

            auto str = colorsv.substr(2, 5);
            std::from_chars(str.data(), str.data() + 5, dig.meters2, 16);
        }
    }

    void part1() {
        auto result{0};

        UnorderedMap<Coord, bool> borders;

        Coord current{0, 0};

        borders[current] = true;

        for (auto &dig : digs) {

            const auto &delta = dirs[dig.direction];

            for (int i = 0; i < dig.meters; i++) {
                current += delta;
                borders[current] = true;
            }
        }

        UnorderedMap<Coord, bool> visited;

        Queue<Coord> q;

        q.push({1, 1});

        while (!q.empty()) {
            const auto coord = q.front();
            q.pop();

            if (visited.find(coord) != visited.end()) {
                continue;
            }

            visited[coord] = true;

            for (auto dir : dirs) {
                auto new_coord = coord + dir;

                if (borders.find(new_coord) == borders.end()) {
                    q.push(new_coord);
                }
            }
        }

        result = borders.size() + visited.size();

        /* for(int y = -100; y < 10; y++) { */
        /*     for(int x = 0; x < 180; x++) { */
        /*         log << (borders.find({x, y})!= borders.end()? '#' : '.'); */
        /*     } */
        /*     log << '\n'; */
        /* } */

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0};

        UnorderedMap<Coord, bool> borders;
        Coord current{0, 0};
        borders[current] = true;

        for (auto &dig : digs) {

            const auto &delta = dirs[dig.direction2];

            for (int i = 0; i < dig.meters2; i++) {
                current += delta;
                borders[current] = true;
            }
        }

        UnorderedMap<Coord, bool> visited;

        Queue<Coord> q;

        q.push({1, 1});

        log << "filling..." << endl;

        while (!q.empty()) {
            const auto coord = q.front();
            q.pop();

            if (visited.find(coord) != visited.end()) {
                continue;
            }

            visited[coord] = true;

            for (auto dir : dirs) {
                auto new_coord = coord + dir;

                if (borders.find(new_coord) == borders.end()) {
                    q.push(new_coord);
                }
            }
        }

        result = borders.size() + visited.size();

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
    /* process("sample.txt"); */
    process("input.txt");
    return 0;
}
