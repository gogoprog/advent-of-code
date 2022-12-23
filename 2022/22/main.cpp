#include "../../common.h"

enum Direction { DRight, DDown, DLeft, DUp };

enum FaceId { Top, Bottom, Right, Left, Front, Back };

static const Vector<Point> &deltas = {Point{1, 0}, Point{0, 1}, Point{-1, 0}, Point{0, -1}};

void rotate(Coord &pos, int count, int size) {
    for (int i = 0; i < count; ++i) {
        auto copy = pos;
        pos = Coord{size - 1 - copy.y, copy.x};
    }
}

struct Connection {
    int id;
    int side;
};

struct Face {
    Map<Coord, char> data;
    Array<Connection, 4> connections;
    Coord origin;
    int rotation{0};
    int oid;

    void rotate(int d, int size) {
        auto copy = data;
        data.clear();
        for (auto kv : copy) {
            auto pos = kv.first;
            ::rotate(pos, d, size);
            data[pos] = kv.second;
        }
        rotation = d;
    }

    void print(const int size) {
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; ++x) {
                log << data[{x, y}];
            }

            log << endl;
        }
    }
};

struct State {
    Coord localPosition;
    int localDirection;
    int face;

    void turn(const char t) {
        switch (t) {
            case 'R':
                localDirection++;
                localDirection %= 4;
                break;
            case 'L':
                localDirection--;
                if (localDirection < 0) {
                    localDirection = 3;
                }
                break;
        }
    }
};

template <int size> struct Context {
    Vector<String> data;
    String commands;
    int width{0}, height;
    Array<Face, 6> faces;
    Coord position;
    int direction{DRight};

    State state;

    void parse(const Strings &lines) {
        for (auto &line : lines) {
            if (line == "")
                break;

            width = std::max<int>(line.size(), width);
            data.push_back(line);
        }

        auto last_line = lines.back();
        commands = last_line;

        for (int c = 0; c < data[0].size(); ++c) {
            if (data[0][c] == '.') {
                position = {c, 0};
                break;
            }
        }

        height = data.size();

        Array<Face, 6> tfaces;

        int f{0};
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 4; ++i) {
                Coord p{i * size, j * size};

                if (isValid(p)) {
                    auto &face = tfaces[f];
                    face.origin = p;
                    f++;
                    face.oid = f;

                    for (int y = 0; y < size; y++) {
                        for (int x = 0; x < size; ++x) {
                            auto p2 = p + Coord{x, y};
                            face.data[{x, y}] = data[p2.y][p2.x];
                        }
                    }
                }
            }
        }

        auto add_connection = [&](auto from, auto side, auto to, auto other_side) {
            faces[from].connections[side] = {to, other_side};
            faces[to].connections[other_side] = {from, side};
        };

        if (size == 4) {
            tfaces[5].rotate(3, size);
            faces[Top] = tfaces[0];
            faces[Front] = tfaces[3];
            faces[Bottom] = tfaces[4];
            faces[Left] = tfaces[2];
            faces[Right] = tfaces[5];
            faces[Back] = tfaces[1];
        } else {
            tfaces[1].rotate(1, size);
            tfaces[3].rotate(1, size);
            tfaces[5].rotate(1, size);
            faces[Top] = tfaces[0];
            faces[Front] = tfaces[2];
            faces[Bottom] = tfaces[4];
            faces[Left] = tfaces[3];
            faces[Right] = tfaces[1];
            faces[Back] = tfaces[5];
        }

        add_connection(Front, DUp, Top, DDown);
        add_connection(Left, DUp, Top, DLeft);
        add_connection(Right, DUp, Top, DRight);
        add_connection(Back, DUp, Top, DUp);

        add_connection(Front, DDown, Bottom, DUp);
        add_connection(Left, DDown, Bottom, DLeft);
        add_connection(Right, DDown, Bottom, DRight);
        add_connection(Back, DDown, Bottom, DDown);

        add_connection(Left, DRight, Front, DLeft);
        add_connection(Front, DRight, Right, DLeft);
        add_connection(Right, DRight, Back, DLeft);
        add_connection(Back, DRight, Left, DLeft);

        state.localPosition = {0, 0};
        state.localDirection = DRight;
        state.face = Top;

        auto test = [&](int fid, int dir, Coord start, int expected_fid, int expected_dir, Coord expected_end) {
            State st;
            st.localPosition = {0, 0};
            st.localDirection = dir;
            st.face = fid;

            while (true) {
                st.localPosition += deltas[st.localDirection];
                wrap2(st);

                if (st.face != fid) {
                    myassert(st.face, expected_fid);
                    myassert(st.localDirection, expected_dir);
                    myassert(st.localPosition, expected_end);
                    return;
                }
            }
        };

        int l = size - 1;
        test(Top, DDown, {0, 0}, Front, DDown, {0, 0});
        test(Top, DLeft, {0, 0}, Left, DDown, {0, 0});
        test(Top, DRight, {0, 0}, Right, DDown, {l, 0});
        test(Top, DUp, {0, 0}, Back, DDown, {l, 0});

        test(Bottom, DDown, {0, 0}, Back, DUp, {l, l});
        test(Bottom, DLeft, {0, 0}, Left, DUp, {l, l});
        test(Bottom, DRight, {0, 0}, Right, DUp, {0, l});
        test(Bottom, DUp, {0, 0}, Front, DUp, {0, l});

        test(Left, DLeft, {0, 0}, Back, DLeft, {l, 0});
        test(Left, DRight, {0, 0}, Front, DRight, {0, 0});
    }

    void wrap(Coord &pos) {
        while (pos.x < 0) {
            pos.x += width;
        }
        while (pos.y < 0) {
            pos.y += height;
        }

        pos.x %= width;
        pos.y %= height;
    }

    bool isValid(const Coord pos) {
        auto line = data[pos.y];
        if (pos.x >= line.size()) {
            return false;
        }

        return line[pos.x] != ' ';
    }

    bool isOpen(const Coord pos) {
        char c = data[pos.y][pos.x];
        return c == '.';
    }

    void goForward(const int count) {
        for (int i = 0; i < count; ++i) {
            auto npos = position + deltas[direction];
            wrap(npos);

            while (!isValid(npos)) {
                npos += deltas[direction];
                wrap(npos);
            }

            if (isOpen(npos)) {
                position = npos;
            } else {
                break;
            }
        }
    }

    void turn(const char t) {
        switch (t) {
            case 'R':
                direction++;
                direction %= 4;
                break;
            case 'L':
                direction--;
                if (direction < 0) {
                    direction = 3;
                }
                break;
        }
    }

    void print() {
        for (auto line : data) {
            log << line << endl;
        }
    }

    void part1() {
        auto result{0};

        String current;

        auto handle_current = [&]() {
            if (!current.empty()) {
                int value = convert<String, int>(current);
                goForward(value);
                current = "";
            }
        };

        for (auto c : commands) {
            auto is_digit = std::isdigit(c);

            if (is_digit) {
                current += c;
            } else {
                handle_current();
                turn(c);
            }
        }

        handle_current();

        result = 1000 * (position.y + 1) + 4 * (position.x + 1) + direction;

        log << "Part1: " << result << endl;
    }

    void wrap2(State &state) {
        auto pos = state.localPosition;
        auto &current_face = faces[state.face];

        int side = -1;

        if (pos.x >= size) {
            side = DRight;
        } else if (pos.x < 0) {
            side = DLeft;
        } else if (pos.y >= size) {
            side = DDown;
        } else if (pos.y < 0) {
            side = DUp;
        }

        if (side != -1) {
            auto conn = &current_face.connections[side];
            pos.x = (pos.x + size) % size;
            pos.y = (pos.y + size) % size;

            auto delta = 2 - (side - conn->side);
            delta = (delta + 4) % 4;

            rotate(pos, delta, size);

            state.localPosition = pos;
            state.face = conn->id;

            state.localDirection = (state.localDirection + delta) % 4;
        }
    }

    bool isOpen(const State &state) {
        char c = faces[state.face].data[state.localPosition];

        return c == '.';
    }

    void goForward2(const int count) {
        for (int i = 0; i < count; ++i) {
            auto nstate = state;
            nstate.localPosition += deltas[state.localDirection];

            wrap2(nstate);

            if (isOpen(nstate)) {
                state = nstate;
            } else {
                break;
            }
        }
    }

    void part2() {
        auto result{0};

        String current;

        auto handle_current = [&]() {
            if (!current.empty()) {
                int value = convert<String, int>(current);
                goForward2(value);
                current = "";
            }
        };

        for (auto c : commands) {
            auto is_digit = std::isdigit(c);

            if (is_digit) {
                current += c;
            } else {
                handle_current();
                state.turn(c);
            }
        }

        handle_current();

        result = compute();

        log << "Part2: " << result << endl;
    }

    void print(const State &state) {
        log << "On face " << faces[state.face].oid << " : " << state.localPosition << endl;
    }

    int compute() {
        auto &face = faces[state.face];
        auto pos = state.localPosition;

        rotate(pos, 4 - face.rotation, size);

        auto final_pos = face.origin + pos;
        return 1000 * (final_pos.y + 1) + 4 * (final_pos.x + 1) + (state.localDirection + 4 - face.rotation) % 4;
    }
};

template <int size> void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        Context<size> context;
        context.parse(lines);
        context.part1();
    }
    {
        Context<size> context;
        context.parse(lines);
        context.part2();
    }
}

int main() {
    process<4>("sample.txt");
    process<50>("input.txt");
    return 0;
}
