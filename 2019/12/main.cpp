#include "../../common.h"

struct Moon {
    Vector3 position;
    Vector3 initial;
    Vector3 velocity;

    void reset() {
        position = initial;
        velocity = {0, 0, 0};
    }

    void load(const String line) {
        InputStringStream iss{line};
        char c;

        iss >> c >> c >> c;

        iss >> position.x;

        iss >> c >> c >> c;

        iss >> position.y;

        iss >> c >> c >> c;

        iss >> position.z;

        initial = position;
    }

    int getPot() {
        return std::abs(position.x) + std::abs(position.y) + std::abs(position.z);
    }

    int getKin() {
        return std::abs(velocity.x) + std::abs(velocity.y) + std::abs(velocity.z);
    }

    bool operator==(const Moon &other) const {
        return position == other.position && velocity == other.velocity;
    }

    bool operator!=(const Moon &other) const {
        return !operator==(other);
    }
};

std::ostream &operator<<(std::ostream &os, const Moon &m) {
    os << "p:" << m.position << ", v:" << m.velocity;
    return os;
}

struct System {
    Vector<Moon> moons;

    void reset() {
        for (auto &moon : moons) {
            moon.reset();
        }
    }

    void applyGravity() {

        for (auto &moon : moons) {
            for (auto &other_moon : moons) {
                if (&moon != &other_moon) {

                    for (int c = 0; c < 3; c++) {
                        if (other_moon.position.coords[c] > moon.position.coords[c]) {
                            moon.velocity.coords[c]++;
                        } else if (other_moon.position.coords[c] < moon.position.coords[c]) {
                            moon.velocity.coords[c]--;
                        }
                    }
                }
            }
        }
    }

    void applyVelocity() {
        for (auto &moon : moons) {
            for (int c = 0; c < 3; c++) {
                moon.position.coords[c] += moon.velocity.coords[c];
            }
        }
    }

    void run() {
        reset();
        for (int i = 0; i < 1000; i++) {
            applyGravity();
            applyVelocity();
        }

        int totalEnergy = 0;

        for (auto &moon : moons) {
            totalEnergy += moon.getPot() * moon.getKin();
        }

        log << "part1: " << totalEnergy << endl;
    }

    void run2() {
        reset();
        bool found{false};

        lli i{0};

        Set<int> values;
        int count{0};

        Vector3 freqs = {-1, -1, -1};

        while (!found) {
            applyGravity();
            applyVelocity();
            ++i;

            for (int c = 0; c < 3; c++) {
                if (freqs.coords[c] == -1) {
                    bool good{true};
                    for (auto &moon : moons) {
                        if (moon.velocity.coords[c] == 0 && moon.position.coords[c] == moon.initial.coords[c]) {
                        } else {
                            good = false;
                            break;
                        }
                    }

                    if (good) {
                        freqs.coords[c] = i;
                        ++count;
                        values.insert(i);

                        if (count == 3) {
                            found = true;
                        }
                    }
                }
            }
        }

        log << values << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    System system;

    for (auto &line : lines) {
        Moon moon;
        moon.load(line);
        system.moons.push_back(moon);
    }

    system.run();

    system.run2();
}

int main() {
    process("sample.txt");
    process("sample2.txt");
    process("input.txt");
    return 0;
}
