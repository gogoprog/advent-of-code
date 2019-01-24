#include "../../common.h"

struct Context {
    int width, height;
    Grid<char> pixels;

    void init() {

        for (int x{0}; x < width; x++) {
            for (int y{0}; y < height; y++) {
                pixels[x][y] = '.';
            }
        }
    }

    void draw() {
        log << "=>" << endl;
        for (int y{0}; y < height; y++) {
            for (int x{0}; x < width; x++) {
                log << pixels[x][y];
            }
            log << endl;
        }
    }

    void rect(const int w, const int h) {
        for (int y{0}; y < h; y++) {
            for (int x{0}; x < w; x++) {
                pixels[x][y] = '#';
            }
        }
    }

    void rotateRow(const int y, const int v) {
        static Vector<char> current;
        current.resize(width);
        for (int x{0}; x < width; x++) {
            current[x] = pixels[x][y];
        }

        for (int x{0}; x < width; x++) {
            int nx = x - v;
            while (nx < 0)
                nx += width;
            nx %= width;
            pixels[x][y] = current[nx];
        }
    }

    void rotateCol(const int x, const int v) {
        static Vector<char> current;
        current.resize(height);
        for (int y{0}; y < height; y++) {
            current[y] = pixels[x][y];
        }

        for (int y{0}; y < height; y++) {
            int nx = y - v;
            while (nx < 0)
                nx += height;
            nx %= height;
            pixels[x][y] = current[nx];
        }
    }

    void part1() {
        int count{0};

        for (int y{0}; y < height; y++) {
            for (int x{0}; x < width; x++) {
                if(pixels[x][y] == '#') {
                    ++count;
                }
            }
        }

        log << "part1: " << count << endl;
    }
};

void process(const String filename, const int w, const int h) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx{w, h};

    ctx.init();
    ctx.draw();

    for (auto line : lines) {
        InputStringStream iss{line};
        String inst;

        iss >> inst;
        if (inst == "rect") {
            int a, b;
            char c;

            iss >> a >> c >> b;

            ctx.rect(a, b);
        } else if (inst == "rotate") {
            String str;
            char xy, c;
            int a, b;
            iss >> str >> xy >> c >> a >> str >> b;

            if (xy == 'x') {
                ctx.rotateCol(a, b);
            } else {
                ctx.rotateRow(a, b);
            }
        }
        ctx.draw();
    }

    ctx.part1();
}

int main() {
    process("sample.txt", 7, 3);
    process("input.txt", 50, 6);
    return 0;
}
