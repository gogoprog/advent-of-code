#include "../../common_fast.h"

enum Directions { U, R, D, L };
constexpr Array<Coord, 4> dirs = {Coord{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

struct Dig {
    int direction;
    int meters;
};

template <> struct std::hash<Coord> {
    std::size_t operator()(const Coord &k) const {
        return k.x + 10000000 * k.y;
    }
};

struct Context {
    Vector<Dig> digs;
    Vector<Dig> digs2;

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

            auto &dig2 = digs2.emplace_back();

            switch (colorsv[7]) {
                case '3':
                    dig2.direction = U;
                    break;
                case '1':
                    dig2.direction = D;
                    break;
                case '2':
                    dig2.direction = L;
                    break;
                case '0':
                    dig2.direction = R;
                    break;
            }

            auto str = colorsv.substr(2, 5);
            std::from_chars(str.data(), str.data() + 5, dig2.meters, 16);
        }
    }

    int solve0(Vector<Dig> &digs) {
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
        /*     for(int x = 0; x < 100; x++) { */
        /*         log << (borders.find({x, y})!= borders.end()? '#' : '.'); */
        /*     } */
        /*     log << '\n'; */
        /* } */

        return result;
    }

    uint64_t solve(Vector<Dig> &digs) {
        uint64_t result{0};

        Vector<Line> lines;

        uint64_t total_removed = 0;
        bool removing = true;

        while (removing) {
            auto len = digs.size();
            removing = false;

            for (int d = len - 1; d >= 0; d--) {

                if (digs[d].meters == 0) {
                    /* digs.erase(digs.begin() + d); */
                    /* log << "Clean dig" << d << endl; */
                }
            }

            len = digs.size();
            /* for (int d = 0; d < len; d++) { */
            /*     auto &dig = digs[d]; */
            /*     auto &dig1 = digs[(d + 1) % len]; */

            /*     if(dig.direction == dig1.direction) { */


            /*         removing = true; */
            /*         break; */
            /*     } */
            /* } */

            if(removing) continue;

            len = digs.size();

            /* lines.resize(len); */

            /* auto current = Point{0, 0}; */
            /* for (int d = 0; d < len; d++) { */
            /*     auto &dig = digs[d]; */
            /*     auto next = current + dirs[dig.direction] * dig.meters; */
            /*     lines[d] = {current, next}; */
            /*     current = next; */
            /* } */

            for (int d = 0; d < len; d++) {
                auto &dig = digs[d];
                auto &dig1 = digs[(d + 1) % len];
                auto &dig2 = digs[(d + 2) % len];

                uint64_t width, height, extracted = 0;

                if ((dig.direction == R && dig1.direction == D && dig2.direction == L) ||
                    (dig.direction == L && dig1.direction == U && dig2.direction == R)) {
                    width = std::min(dig.meters, dig2.meters);
                    height = dig1.meters + 1;
                    extracted = width * height;

                    dig.meters -= width;
                    dig2.meters -= width;

                    total_removed += extracted;
                }

                if ((dig.direction == D && dig1.direction == L && dig2.direction == U) ||
                    (dig.direction == U && dig1.direction == R && dig2.direction == D)) {
                    width = dig1.meters + 1;
                    height = std::min(dig.meters, dig2.meters);

                    extracted = width * height;

                    dig.meters -= height;
                    dig2.meters -= height;

                    total_removed += extracted;
                }

                if (extracted) {
                    /* log << "Removed " << width << " x " << height << " = " << extracted << " (" << total_removed <<
                     * ")" */
                    /*     << endl; */
                    removing = true;
                    break;
                }
            }
        }

        result = total_removed;

        log << "Total removed: " << total_removed << endl;
        log << "Rest: " << digs.size() << endl;

        if (1 && digs.size() > 0) {

            UnorderedMap<Coord, bool> borders;

            Coord current{0, 0};

            borders[current] = true;

            for (auto &dig : digs) {

                const auto &delta = dirs[dig.direction];

                for (int i = 0; i < dig.meters; i++) {
                    current += delta;
                    borders[current] = true;

                    log << current << endl;
                }
            }

            for (int y = -10; y < 10; y++) {
                for (int x = -10; x < 20; x++) {
                    log << (borders.find({x, y}) != borders.end() ? '#' : '.');
                }
                log << '\n';
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

            result += borders.size() + visited.size();
        }

        return result;
    }

    void part1() {
        auto result = solve0(digs);

        auto r2 = solve(digs);
        log << "Part1: " << r2 << endl;
        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result = solve(digs2);
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
    /* process("input.txt"); */
    return 0;
}
