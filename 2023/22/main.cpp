#include "../../common_fast.h"

struct Brick {
    Point3 position;
    Point3 extent;
};

struct Context {

    Vector<Brick> initialBricks;

    void parse(auto lines) {

        auto to_point3s = rv::transform([](auto line) {
            Point3 p;

            auto numbers = line | rv::split_sv(',') | rv::to_ints;

            p.x = numbers | rv::get0;
            p.y = numbers | rv::get1;
            p.z = numbers | rv::get2;

            return p;
        });

        auto to_bricks = rv::transform([&](auto line) {
            Brick b;

            auto parts = line | rv::split_sv('~') | to_point3s;

            auto start = parts | rv::get0;
            auto end = parts | rv::get1;

            b.extent = end - start;
            b.position = start;

            return b;
        });

        rs::copy(lines | to_bricks, std::back_inserter(initialBricks));

        for (auto b : initialBricks) {
            /* log << b.position << " -> " << b.extent << "\n"; */
        }
    }

    void part1() {

        auto len = initialBricks.size();

        Vector<Point3> positions;
        Vector<Point3> extents;

        for (auto b : initialBricks) {
            positions.push_back(b.position);
            extents.push_back(b.extent);
        }

        auto collides = [&](auto &pos, auto &extent, auto other_index) {
            const auto &other_pos = positions[other_index];
            const auto &other_extent = extents[other_index];

            for (auto c = 0; c < 3; c++) {

                auto min1 = pos[c];
                auto max1 = pos[c] + extent[c];
                auto min2 = other_pos[c];
                auto max2 = other_pos[c] + other_extent[c];

                if (!(std::max(min1, min2) <= std::min(max1, max2))) {

                    return false;
                }
            }

            return true;
        };

        auto collides_all = [&](auto &pos, auto &extent, auto exclude_index, auto exclude_index2) {
            for (auto j = 0; j < len; j++) {

                if (j != exclude_index && j != exclude_index2) {

                    if (collides(pos, extent, j)) {
                        return true;
                    }
                }
            }

            if (pos.z < 0)
                return true;

            return false;
        };

        bool falling = true;

        while (falling) {
            falling = false;

            for (auto i = 0; i < len; i++) {
                auto new_pos = positions[i];
                auto &extent = extents[i];

                new_pos.z -= 1;

                if (!collides_all(new_pos, extent, i, -1)) {
                    positions[i] = new_pos;
                    falling = true;
                    /* break; */
                }
            }
        }

        log << "Fall complete.\n";

        auto result{0};
        for (auto b = 0; b < len; b++) {
            falling = false;

            for (auto i = 0; i < len; i++) {
                auto new_pos = positions[i];
                auto &extent = extents[i];

                new_pos.z -= 1;

                if (!collides_all(new_pos, extent, i, b)) {
                    falling = true;
                    break;
                }
            }

            if (!falling) {
                result += 1;
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto len = initialBricks.size();

        Vector<Point3> positions;
        Vector<Point3> extents;

        for (auto b : initialBricks) {
            positions.push_back(b.position);
            extents.push_back(b.extent);
        }

        auto collides = [&](auto &pos, auto &extent, auto other_index) {
            const auto &other_pos = positions[other_index];
            const auto &other_extent = extents[other_index];

            for (auto c = 0; c < 3; c++) {

                auto min1 = pos[c];
                auto max1 = pos[c] + extent[c];
                auto min2 = other_pos[c];
                auto max2 = other_pos[c] + other_extent[c];

                if (!(std::max(min1, min2) <= std::min(max1, max2))) {

                    return false;
                }
            }

            return true;
        };

        auto collides_all = [&](auto &pos, auto &extent, auto exclude_index, auto exclude_index2) {
            for (auto j = 0; j < len; j++) {

                if (j != exclude_index && j != exclude_index2) {

                    if (collides(pos, extent, j)) {
                        return true;
                    }
                }
            }

            if (pos.z < 0)
                return true;

            return false;
        };

        bool falling = true;

        while (falling) {
            falling = false;

            for (auto i = 0; i < len; i++) {
                auto new_pos = positions[i];
                auto &extent = extents[i];

                new_pos.z -= 1;

                if (!collides_all(new_pos, extent, i, -1)) {
                    positions[i] = new_pos;
                    falling = true;
                    /* break; */
                }
            }
        }

        log << "Fall complete.\n";

        auto positions_copy = positions;

        Set<int> fallings;

        auto result{0};

        for (auto b = 0; b < len; b++) {
            falling = true;
            fallings.clear();
            positions = positions_copy;

            while (falling) {
                falling = false;

                for (auto i = 0; i < len; i++) {
                    auto new_pos = positions[i];
                    auto & extent = extents[i];

                    new_pos.z -= 1;

                    if (!collides_all(new_pos, extent, i, b)) {
                        fallings.insert(i);
                        positions[i] = new_pos;
                        falling = true;
                        /* break; */
                    }
                }
            }

            result += fallings.size();
        }

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
