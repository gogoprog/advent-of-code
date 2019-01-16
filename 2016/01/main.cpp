#include "../../common.h"

enum Direction { N = 0, E, S, W };

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename, ',');

    for (auto &line : lines) {
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
    }

    int x{0}, y{0};
    int d{N};
    Grid<bool> visited;
    bool first{true};

    visited[0][0] = true;

    for (auto &line : lines) {
        InputStringStream iss{line};
        char c;

        iss >> c;

        if (c == 'R') {
            d++;
            d %= 4;
        } else {
            d--;
            if (d < 0) {
                d += 4;
            }
        }

        int steps;

        iss >> steps;

        for (int s{0}; s < steps; ++s) {

            switch (d) {
            case N:
                y -= 1;
                break;
            case E:
                x += 1;
                break;
            case S:
                y += 1;
                break;
            case W:
                x -= 1;
                break;
            }

            if (first && visited[x][y]) {
                log << "part2: " << x << ", " << y << " = " << (abs(x) + abs(y)) << endl;
                first = false;
            }

            visited[x][y] = true;
        }
    }

    log << "part1: " << x << ", " << y << " = " << (abs(x) + abs(y)) << endl;
}

int main() {
    process("sample.txt");
    process("sample2.txt");
    process("input.txt");
    return 0;
}
