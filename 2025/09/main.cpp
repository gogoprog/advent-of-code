#include "../../common_fast.h"

static const Vector<Coord> &deltas = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

struct Context {

    using Line = Pair<Coord, Coord>;
    Vector<Line> hlines;
    Vector<Line> vlines;
    Vector<Coord> tiles;

    void parse(auto lines) {
        for (auto line : lines) {
            Coord c;
            c.x = line | rv::split_sv(',') | rv::get0 | rv::to_int;
            c.y = line | rv::split_sv(',') | rv::get1 | rv::to_int;

            tiles.push_back(c);
        }

        auto len = tiles.size();
        for (int t = 0; t < len; t++) {
            auto curr = tiles[t];
            auto next = tiles[(t + 1) % len];

            if (curr.x == next.x) {
                vlines.push_back({curr, next});
            } else {
                hlines.push_back({curr, next});
            }
        }
    }

    void part1() {
        Uint64 result{0};

        for (auto &tile : tiles) {
            for (auto &other : tiles) {

                auto dx = other.x - tile.x;
                auto dy = other.y - tile.y;

                if (dx < 0)
                    dx = -dx;
                if (dy < 0)
                    dy = -dy;

                dx++;
                dy++;

                Uint64 area = Uint64(dx) * Uint64(dy);

                if (area > result) {
                    result = area;
                }
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        Uint64 result{0};

        auto len = tiles.size();

        Map<Coord, bool> perimeter;

        for (int t = 0; t < len; t++) {
            auto curr = tiles[t];
            auto next = tiles[(t + 1) % len];

            if (curr.x == next.x) {
                auto max = std::max(curr.y, next.y);
                for (int y = std::min(curr.y, next.y); y <= max; ++y) {
                    perimeter[{curr.x, y}] = true;
                }
            } else {
                auto max = std::max(curr.x, next.x);
                for (int x = std::min(curr.x, next.x); x <= max; ++x) {
                    perimeter[{x, curr.y}] = true;
                }
            }
        }

        auto is_inside = [&](const auto pos) {
            if (perimeter.find(pos) != perimeter.end()) {
                return true;
            }

            Line best;
            int bestx = 0;

            for (auto &line : vlines) {
                auto miny = std::min(line.first.y, line.second.y);
                auto maxy = std::max(line.first.y, line.second.y);
                if (miny <= pos.y && pos.y <= maxy) {
                    if (line.first.x > bestx && line.first.x < pos.x) {
                        bestx = line.first.x;
                        best = line;
                    }
                }
            }

            if (bestx) {
                if (best.first.y > best.second.y) {
                    return true;
                }
            }

            return false;
        };

        for (auto &a : tiles) {
            for (auto &c : tiles) {

                auto minx = std::min(a.x, c.x);
                auto maxx = std::max(a.x, c.x);
                auto miny = std::min(a.y, c.y);
                auto maxy = std::max(a.y, c.y);

                Uint64 area = Uint64(maxx - minx + 1) * Uint64(maxy - miny + 1);

                if (area > result) {

                    if (is_inside(Coord{minx, miny}) && is_inside(Coord{maxx, miny}) && is_inside(Coord{maxx, maxy}) &&
                        is_inside(Coord{minx, maxy})) {

                        bool valid = true;

                        for (int x = minx; x <= maxx; x++) {
                            if (!is_inside(Coord{x, miny}) || !is_inside(Coord{x, maxy})) {
                                valid = false;
                                break;
                            }
                        }

                        if (!valid)
                            continue;

                        for (int y = miny; y <= maxy; y++) {
                            if (!is_inside(Coord{minx, y}) || !is_inside(Coord{maxx, y})) {
                                valid = false;
                                break;
                            }
                        }

                        if (valid) {
                            result = area;
                        }
                    }
                }
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
