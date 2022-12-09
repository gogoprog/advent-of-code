#include "raylib.h"
#include "raymath.h"

#include "../../common.h"

Map<char, Coord> deltas{{'R', Coord{1, 0}}, {'L', {-1, 0}}, {'U', {0, -1}}, {'D', {0, 1}}};

template <int size> struct Context {
    Map<Coord, bool> visited;
    Array<Coord, size> knots;

    void init() {
        visited[{0, 0}] = true;

        for (auto &knot : knots) {
            knot.set(0, 0);
        }
    }

    void follow(const int index, const Coord parent_move) {
        auto parent = knots[index - 1];
        auto &current = knots[index];

        auto diff = parent - current;

        Coord move{0, 0};

        /* log << index << " " << diff << endl; */

        if (std::abs(diff.x) > 1 || std::abs(diff.y) > 1) {
            if (parent.x == current.x || parent.y == current.y) {
                move = diff / diff.manhattan();
            } else {
                if (diff.manhattan() == 4) {
                    move = diff / 2;
                } else if (std::abs(diff.y) > 1) {
                    move.x = diff.x;
                    move.y = parent_move.y;
                } else if (std::abs(diff.x) > 1) {
                    move.x = parent_move.x;
                    move.y = diff.y;
                }
            }
        }

        if (move.manhattan() != 0) {
            current += move;

            /* draw(); */

            if (index < size - 1) {
                follow(index + 1, move);
            }
        }
    }

    void move(const char direction, const int length) {
        auto delta = deltas[direction];
        /* log << "== " << direction << " " << length << " == " << endl; */

        for (auto i : rs::iota_view(0, length)) {
            knots[0] += delta;

            follow(1, delta);

            visited[knots.back()] = true;

            /* draw(); */
        }
    }

    void draw() {

        log << endl;
        auto [minPoint, maxPoint] = getMinMax(visited);

        for (int y = minPoint.y - size; y <= maxPoint.y + size; y++) {
            for (int x = minPoint.x - size; x <= maxPoint.x + size; ++x) {
                auto pos = Coord{x, y};
                bool found = false;

                for (auto i : rs::iota_view(0, size)) {
                    if (pos == knots[i]) {
                        log << i;
                        found = true;
                        break;
                    }
                }

                if (!found) {

                    if (pos == Coord{0, 0}) {
                        log << 's';
                    } else {
                        log << '.';
                    }
                }
            }

            log << endl;
        }
    }

    int countVisited() const {
        return std::count_if(visited.begin(), visited.end(), [](auto kv) { return kv.second; });
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    {
        Context<2> ctx;

        ctx.init();

        for (auto line : lines) {
            char d;
            int length;
            InputStringStream iss{line};
            iss >> d >> length;
            ctx.move(d, length);
        }

        log << "Part1: " << ctx.countVisited() << endl;
    }
    {
        Context<10> ctx;

        ctx.init();

        for (auto line : lines) {
            char d;
            int length;
            InputStringStream iss{line};
            iss >> d >> length;
            ctx.move(d, length);
        }

        log << "Part2: " << ctx.countVisited() << endl;
    }
}

int main() {
    process("sample.txt");
    process("sample2.txt");
    process("input.txt");
    return 0;
}
