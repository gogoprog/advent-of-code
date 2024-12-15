#include "../../common_fast.h"

struct Context {

    struct State {
        Coords boxes;
        Coord robot;
    };

    Grid grid;
    StringViews moves;
    State initialState;

    inline bool isWall(const Coord coord) const {
        return grid.get(coord) == '#';
    }

    void draw(const State &state) const {

        for (int y = 0; y < grid.height; ++y) {
            for (int x = 0; x < grid.width; ++x) {

                auto coord = Coord{x, y};

                if (std::find(state.boxes.begin(), state.boxes.end(), coord) != state.boxes.end()) {
                    log << 'O';
                } else if (isWall(coord)) {
                    log << '#';
                } else if (coord == state.robot) {
                    log << '@';
                } else {
                    log << '.';
                }
            }
            log << endl;
        }
    }

    void attempt(State &state, char c) {
        static Map<char, Coord> directions = {{'<', Coord{-1, 0}}, {'>', {1, 0}}, {'^', {0, -1}}, {'v', {0, 1}}};

        auto delta = directions[c];

        auto next_pos = state.robot + delta;

        if (!isWall(next_pos)) {
            for (auto &box : state.boxes) {
                if (box == next_pos) {
                    if (tryMove(state, box, delta)) {
                        state.robot = next_pos;
                    }
                    return;
                }
            }

            state.robot = next_pos;
        }
    }

    bool tryMove(State &state, Coord &box, Coord delta) {
        auto next_pos = box + delta;

        if (isWall(next_pos)) {
            return false;
        } else {
            for (auto &other_box : state.boxes) {
                if (other_box == next_pos) {
                    if (tryMove(state, other_box, delta)) {
                        box = next_pos;
                        return true;
                    }
                    return false;
                }
            }
        }

        box = next_pos;

        return true;
    }

    Int64 compute(const State &state) {
        Int64 result{0};
        for (auto box : state.boxes) {
            result += box.y * 100 + box.x;
        }
        return result;
    }

    void parse(auto content) {
        auto parts = content | rv::split_sv("\n\n");

        grid.parse(parts | rv::get0 | rv::split_sv('\n'));

        rs::copy(parts | rv::get1 | rv::split_sv('\n'), std::back_inserter(moves));

        grid.for_each([&](auto coord, char c) {
            if (c == 'O') {
                initialState.boxes.push_back(coord);
            } else if (c == '@') {
                initialState.robot = coord;
            }
            return true;
        });
    }

    void part1() {
        auto result{0_int64};

        State state = initialState;

        /* draw(state); */

        for (auto line : moves) {
            for (auto c : line) {
                /* log << c << endl; */
                attempt(state, c);
                /* draw(state); */
            }
        }

        result = compute(state);
        log << "Part1: " << result << endl;
    }

    inline bool isWall2(const Coord coord) const {
        auto coord2 = Coord{coord.x / 2, coord.y};
        return grid.get(coord2) == '#';
    }

    void draw2(const State &state) const {

        for (int y = 0; y < grid.height; ++y) {
            for (int x = 0; x < grid.width * 2; ++x) {

                auto coord = Coord{x, y};

                if (std::find(state.boxes.begin(), state.boxes.end(), coord) != state.boxes.end()) {
                    log << '[';
                } else if (std::find(state.boxes.begin(), state.boxes.end(), Coord{x - 1, y}) != state.boxes.end()) {
                    log << ']';
                } else if (isWall2(coord)) {
                    log << '#';
                } else if (coord == state.robot) {
                    log << '@';
                } else {
                    log << '.';
                }
            }
            log << endl;
        }
    }

    void attempt2(State &state, char c) {
        static Map<char, Coord> directions = {{'<', Coord{-1, 0}}, {'>', {1, 0}}, {'^', {0, -1}}, {'v', {0, 1}}};

        auto delta = directions[c];

        auto next_pos = state.robot + delta;

        if (!isWall2(next_pos)) {
            for (auto &box : state.boxes) {
                if (box == next_pos) {
                    if (canMove2(state, box, delta)) {
                        applyMove2(state, box, delta);
                        state.robot = next_pos;
                    }
                    return;
                } else if (Coord{box.x + 1, box.y} == next_pos) {
                    if (canMove2(state, box, delta)) {
                        applyMove2(state, box, delta);
                        state.robot = next_pos;
                    }
                    return;
                }
            }

            state.robot = next_pos;
        }
    }

    bool boxesCollide(const Coord &a, const Coord &b) const {
        return std::abs(a.x - b.x) < 2 && std::abs(a.y - b.y) < 1;
    }

    bool canMove2(State &state, Coord &box, Coord delta) {
        auto next_pos = box + delta;
        auto next_pos2 = next_pos;
        next_pos2.x++;

        if (isWall2(next_pos) || isWall2(next_pos2)) {
            return false;
        }

        bool result = true;
        for (auto &other_box : state.boxes) {
            if (box != other_box) {
                if (boxesCollide(next_pos, other_box)) {
                    result = result && canMove2(state, other_box, delta);
                }
            }
        }

        return result;
    }

    void applyMove2(State &state, Coord &box, Coord delta) {
        auto next_pos = box + delta;
        auto next_pos2 = next_pos;
        next_pos2.x++;

        for (auto &other_box : state.boxes) {
            if (box != other_box) {
                if (boxesCollide(next_pos, other_box)) {
                    applyMove2(state, other_box, delta);
                }
            }
        }

        box = next_pos;
    }

    void part2() {
        auto result{0_int64};

        State state = initialState;

        for (auto &box : state.boxes) {
            box.x *= 2;
        }

        state.robot.x *= 2;

        /* draw2(state); */

        for (auto line : moves) {
            for (auto c : line) {
                /* log << c << endl; */
                attempt2(state, c);
                /* draw2(state); */
            }
        }
        draw2(state);

        result = compute(state);

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    {
        Context context;
        context.parse(content);
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample3.txt");
    process("sample2.txt");
    process("sample.txt");
    process("input.txt");
    return 0;
}
