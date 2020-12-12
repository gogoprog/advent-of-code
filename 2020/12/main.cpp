#include "../../common.h"

enum Direction { N, E, S, W };
static const Vector<Point> &deltas = {Point{0, 1}, Point{1, 0}, Point{0, -1}, Point{-1, 0}};

struct Ship {

    int direction{E};
    Point position{0, 0};

    Point waypoint{10, 1};
    Point position2{0, 0};

    void process(const String line) {
        InputStringStream iss{line};

        char action;
        int count;

        iss >> action >> count;

        switch (action) {
            case 'N': {
                position.y += count;
                waypoint.y += count;
            } break;
            case 'S': {
                position.y -= count;
                waypoint.y -= count;
            } break;
            case 'E': {
                position.x += count;
                waypoint.x += count;
            } break;
            case 'W': {
                position.x -= count;
                waypoint.x -= count;
            } break;
            case 'L': {
                int n = count / 90;
                direction -= n;
                while (direction < 0) {
                    direction += 4;
                }

                for (int i{0}; i < n; i++) {
                    auto tmp = waypoint;
                    waypoint = {-tmp.y, tmp.x};
                }
            } break;
            case 'R': {
                int n = count / 90;
                direction += n;
                direction %= 4;
                for (int i{0}; i < n; i++) {
                    auto tmp = waypoint;
                    waypoint = {tmp.y, -tmp.x};
                }
            } break;
            case 'F': {
                position += deltas[direction] * count;
                position2 += waypoint * count;
            } break;
        }
    }

    int manhattan() const {
        return abs(position.x) + abs(position.y);
    }

    int manhattan2() const {
        return abs(position2.x) + abs(position2.y);
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Ship ship;

    for (auto &line : lines) {
        ship.process(line);
    }

    log << "Part1: " << ship.manhattan() << endl;
    log << "Part2: " << ship.manhattan2() << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
