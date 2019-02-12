#include "../../common.h"

struct Disc {
    int positions;
    int startPos;
    int currentPos;
};

struct Context {
    Vector<Disc> discs;

    void parse(String line) {
        InputStringStream iss{line};
        String str;
        int npos, cpos;

        iss >> str >> str >> str >> npos >> str >> str >> str >> str >> str >> str >> str >> cpos;

        discs.push_back({npos, cpos, cpos});
    }

    void reset() {
        for (auto &disc : discs) {
            disc.currentPos = disc.startPos;
        }
    }

    bool passes(const int t) {
        int time{t};

        for (auto &disc : discs) {
            time++;

            disc.currentPos += time;

            disc.currentPos %= disc.positions;

            if (disc.currentPos != 0) {
                return false;
            }
        }

        return true;
    }

    void run() {

        int i{0};

        while (true) {
            reset();

            bool r = passes(i);

            if (r) {
                log << "passes with " << i << endl;
                break;
            }

            ++i;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx;

    for (auto line : lines) {
        ctx.parse(line);
    }

    log << "part1" << endl;
    ctx.run();

    log << "part2" << endl;
    ctx.discs.push_back({11, 0, 0});
    ctx.run();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
