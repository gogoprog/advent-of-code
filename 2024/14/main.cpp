#include "../../common_fast.h"

template <int width, int height> struct Context {

    struct Robot {
        Coord position;
        Coord velocity;
    };

    Vector<Robot> robots0;

    void parse(auto lines) {

        for (auto line : lines) {
            robots0.push_back({});
            auto &robot = robots0.back();

            auto parts = line | rv::split_sv(' ');

            auto p_v = parts | rv::get0 | rv::drop(2) | rv::split_sv(',') | rv::to_int32s;
            auto v_v = parts | rv::get1 | rv::drop(2) | rv::split_sv(',') | rv::to_int32s;

            robot.position = {p_v | rv::get0, p_v | rv::get1};
            robot.velocity = {v_v | rv::get0, v_v | rv::get1};
        }
    }

    void draw(Map<Coord, int> &counts) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                auto c = counts[{x, y}];
                if (c) {
                    log << c;
                } else {
                    log << '.';
                }
            }
            log << endl;
        }
    }

    Int64 compute(Map<Coord, int> &counts) {
        int quads[4] = {0, 0, 0, 0};
        int midh = height / 2;
        int midw = width / 2;
        for (auto &kv : counts) {
            auto pos = kv.first;
            if (kv.second) {
                if (pos.x != midw && pos.y != midh) {
                    int x = pos.x < midw ? 0 : 1;
                    int y = pos.y < midh ? 0 : 1;

                    quads[x + 2 * y] += kv.second;
                }
            }
        }

        return quads[0] * quads[1] * quads[2] * quads[3];
    }

    bool isFullMid(Map<Coord, int> &counts) {

        auto r = 0;
        int midh = height / 2;
        int midw = width / 2;

        for (int y = 0; y < height; ++y) {
            r += counts[Coord{midw, y}];
        }

        return r > height / 3;
    }

    bool isArranged(Map<Coord, int> &counts) {
        int quads[4] = {0, 0, 0, 0};
        int midh = height / 2;
        int midw = width / 2;
        for (auto &kv : counts) {
            auto pos = kv.first;
            if (kv.second) {
                if (pos.x != midw && pos.y != midh) {
                    int x = pos.x < midw ? 0 : 1;
                    int y = pos.y < midh ? 0 : 1;

                    quads[x + 2 * y] += kv.second;
                }
            }
        }

        return (quads[0] + quads[1]) * 2 < quads[2] + quads[3];
    }

    bool isArranged2(Map<Coord, int> &counts) {
        for (auto &kv : counts) {
            auto pos = kv.first;
            if (kv.second) {

                bool good = true;

                for (int i = 0; i < height / 6; ++i) {

                    if (counts[{pos.x, pos.y + i}] == 0) {
                        good = false;
                        break;
                    }
                }

                if (good) {
                    return true;
                }
            }
        }

        return false;
    }

    void part1() {
        auto robots = robots0;

        Map<Coord, int> counts;

        for (auto &robot : robots) {
            auto &pos = robot.position;
            pos += robot.velocity * 100;

            while (pos.x < 0) {
                pos.x += width;
            }

            while (pos.y < 0) {
                pos.y += height;
            }

            pos.x %= width;
            pos.y %= height;

            counts[pos]++;
        }

        auto result = compute(counts);

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0_int64};

        auto robots = robots0;
        int i = 1;

        while (true) {

            Map<Coord, int> counts;

            for (auto &robot : robots) {
                auto &pos = robot.position;
                pos += robot.velocity;

                while (pos.x < 0) {
                    pos.x += width;
                }

                while (pos.y < 0) {
                    pos.y += height;
                }

                pos.x %= width;
                pos.y %= height;

                counts[pos]++;
            }

            if (isArranged2(counts)) {
                result = i;
                draw(counts);
                break;
            }
            ++i;
        }

        log << "Part2: " << result << endl;
    }
};

template <int width, int height> void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context<width, height> context;
        context.parse(lines);
        context.part1();
        context.part2();
    }
}

int main() {
    process<11, 7>("sample.txt");
    process<101, 103>("input.txt");
    return 0;
}
