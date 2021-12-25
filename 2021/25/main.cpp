#include "../../common.h"

struct Context {
    Vector<String> lines;
    Map<Point, bool> moved;

    int width;
    int height;

    void init() {
        width = lines[0].size();
        height = lines.size();
    }

    void draw() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                log << lines[y][x];
            }
            log << endl;
        }
    }

    void run() {

        int i = 0;

        auto previous = lines;
        auto inter = lines;

        /* draw(); */

        while (true) {

            previous = lines;

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    moved[{x, y}] = false;
                }
            }

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    auto next_x = (x + 1) % width;
                    if (!moved[{x, y}] && lines[y][x] == '>') {
                        if (previous[y][next_x] == '.') {
                            lines[y][next_x] = '>';
                            lines[y][x] = '.';
                            moved[{next_x, y}] = true;
                        }
                    }
                }
            }


            inter = lines;

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    auto next_y = (y + 1) % height;
                    if (!moved[{x, y}] && lines[y][x] == 'v') {
                        if (inter[next_y][x] == '.') {
                            lines[y][x] = '.';
                            lines[next_y][x] = 'v';
                            moved[{x, next_y}] = true;
                        }
                    }
                }
            }

            if (previous == lines) {
                log << "Part1: " << i + 1 << endl;
                break;
            }

            ++i;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    Context ctx{lines};
    ctx.init();
    ctx.run();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
