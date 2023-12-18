#include "../../common_fast.h"

enum Directions { U, R, D, L };
constexpr Array<Coord, 4> dirs = {Coord{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

struct Dig {
    int direction;
    uint64_t meters;
};

struct Rect {
    Coord origin;
    Coord size;

    bool contains(Coord c) const {
        return origin.x <= c.x && c.x <= origin.x + size.x && origin.y <= c.y && c.y <= origin.y + size.y;
    }

    bool hit(Line l) const {
        /* return contains(l.first) || contains(l.second); */
        return contains(l.first);
    }
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
                    digs.erase(digs.begin() + d);
                }
            }

            len = digs.size();

            if (len > 1)
                for (int d = 0; d < len; d++) {
                    auto &dig = digs[d];
                    auto &dig1 = digs[(d + 1) % len];

                    if (dig.direction == dig1.direction) {

                        dig1.meters += dig.meters;
                        digs.erase(digs.begin() + d);
                        removing = true;
                        break;
                    }
                }

            if (removing)
                continue;

            len = digs.size();

            lines.resize(len);

            auto current = Point{0, 0};
            for (int d = 0; d < len; d++) {
                auto &dig = digs[d];
                auto next = current + dirs[dig.direction] * dig.meters;
                lines[d] = {current, next};
                current = next;
            }

            if (len > 4)
                for (int d = 0; d < len; d++) {

                    auto d1 = (d + 1) % len;
                    auto d2 = (d + 2) % len;
                    auto d3 = (d + 3) % len;
                    auto &dig = digs[d];
                    auto &dig1 = digs[d1];
                    auto &dig2 = digs[d2];
                    auto corner = lines[d].second;

                    int width, height;
                    int64_t extracted = 0;

                    if ((dig.direction == R && dig1.direction == D && dig2.direction == L) ||
                        (dig.direction == L && dig1.direction == U && dig2.direction == R)) {

                        Rect rect;

                        width = std::min(dig.meters, dig2.meters);
                        height = dig1.meters + 1;

                        if (dig.direction == R) {
                            rect.origin.x = corner.x - width;
                            rect.origin.y = corner.y;

                        } else {
                            rect.origin.x = corner.x;
                            rect.origin.y = corner.y - height;
                        }

                        rect.size = {width, height};

                        bool skip = false;

                        for (auto l2 = 0; l2 < len; l2++) {
                            if (l2 != d && l2 != d1 && l2 != d2 && l2 != d3) {

                                if (rect.hit(lines[l2])) {
                                    skip = true;
                                    break;
                                }
                            }
                        }

                        if (!skip) {

                            extracted = width * (int64_t)height;
                            dig.meters -= width;
                            dig2.meters -= width;

                            total_removed += extracted;
                        }
                    }

                    else if ((dig.direction == D && dig1.direction == L && dig2.direction == U) ||
                             (dig.direction == U && dig1.direction == R && dig2.direction == D)) {

                        Rect rect;

                        width = dig1.meters + 1;
                        height = std::min(dig.meters, dig2.meters);

                        if (dig.direction == U) {
                            rect.origin.x = corner.x;
                            rect.origin.y = corner.y;

                        } else {
                            rect.origin.x = corner.x - width;
                            rect.origin.y = corner.y - height;
                        }

                        rect.size = {width, height};

                        bool skip = false;

                        for (auto l2 = 0; l2 < len; l2++) {
                            if (l2 != d && l2 != d1 && l2 != d2 && l2 != d3) {

                                if (rect.hit(lines[l2])) {
                                    skip = true;

                                    /* log << "skip!" << endl; */
                                    break;
                                }
                            }
                        }

                        if (!skip) {

                            extracted = width * (int64_t)height;

                            dig.meters -= height;
                            dig2.meters -= height;

                            total_removed += extracted;
                        }
                    }

                    if (extracted) {
                        /* log << "Removed " << width << " x " << height << " = " << extracted << " (" << total_removed
                         */
                        /*     << ")" << endl; */
                        removing = true;
                        break;
                    }
                }
        }

        result = total_removed;

        /* log << "Total removed: " << total_removed << endl; */
        /* log << "Rest: " << digs.size() << endl; */

        if (digs.size() == 4) {
            result += (digs[0].meters + 1) * (digs[1].meters + 1);
        } else {
            log << "Whoops?!" << endl;
        }
        return result;
    }

    void part1() {
        auto result = solve(digs);

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
    process("input.txt");
    return 0;
}
