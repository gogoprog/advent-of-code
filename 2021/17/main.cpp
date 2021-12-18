#include "../../common.h"

struct Probe {
    Point velocity;
    Point position{0, 0};

    int highest{0};

    void update() {
        position.x += velocity.x;
        position.y += velocity.y;

        if (velocity.x > 0) {
            velocity.x -= 1;
        } else if (velocity.x < 0) {
            velocity.x += 1;
        }

        velocity.y -= 1;

        highest = std::max(highest, position.y);
    }
};

struct Context {
    Point min, max;

    void run() {

        int highest = 0;

        Set<Point> valid_velocities;

        for (int vx = 1; vx < 1000; ++vx) {
            for (int vy = -10000; vy < 10000; ++vy) {

                Point initial_velocity = {vx, vy};

                Probe probe{initial_velocity};

                bool valid = false;

                int steps = 0;
                while (steps < 10000) {
                    probe.update();

                    if (isWithin(probe.position)) {
                        valid = true;
                        break;
                    }

                    if (isLost(probe)) {
                        break;
                    }

                    steps++;
                }

                if (valid) {
                    highest = std::max(highest, probe.highest);
                    valid_velocities.insert(initial_velocity);
                }
            }
        }

        log << "Part1: " << highest << endl;
        log << "Part2: " << valid_velocities.size() << endl;
    }

    bool isWithin(const Point &other) const {
        return other.x >= min.x && other.x <= max.x && other.y >= min.y && other.y <= max.y;
    }

    bool isLost(const Probe &probe) const {
        if (probe.position.x > max.x) {
            return true;
        }

        if (probe.position.y < min.y) {
            return true;
        }

        return false;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto line = lines[0];
    InputStringStream iss{line};
    String str;
    char c;

    Context ctx;

    iss >> str >> str >> c >> c >> ctx.min.x >> c >> c >> ctx.max.x >> c >> c >> c >> ctx.min.y >> c >> c >> ctx.max.y;

    ctx.run();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
