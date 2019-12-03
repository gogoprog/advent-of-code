#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    int best = 1000000;

    Grid<bool> visited;
    Grid<int> steps;

    bool first = true;

    for (auto line : lines) {

        auto items = splitString(line, ',');

        int x = 0;
        int y = 0;

        int s = 0;

        for (auto item : items) {
            InputStringStream iss{item};
            char c;
            int v;

            iss >> c;
            iss >> v;

            for (int i = 0; i < v; ++i) {

                switch (c) {

                    case 'U':
                        y += 1;
                        break;
                    case 'D':
                        y -= 1;
                        break;
                    case 'R':
                        x += 1;
                        break;
                    case 'L':
                        x -= 1;
                        break;
                }

                ++s;

                if (!first && visited[x][y]) {
                    steps[x][y] += s;

                    if (steps[x][y] < best) {
                        best = steps[x][y];
                        log << best << endl;
                    }
                }

                if (first && !visited[x][y]) {
                    visited[x][y] = true;
                    steps[x][y] = s;
                }
            }
        }

        first = false;
    }
}

int main() {
    process("input.txt");
    return 0;
}
