#include "../../common.h"

struct State {
    Map<int, int> lastSpoken;
    Map<int, int> preLastSpoken;
    int turn{0};

    int last;

    inline void add(const int value) {
        turn++;
        preLastSpoken[value] = lastSpoken[value];
        lastSpoken[value] = turn;
        last = value;
    }

    inline void process() {
        if (preLastSpoken[last] == 0) {
            add(0);
        } else {
            auto v = lastSpoken[last] - preLastSpoken[last];
            add(v);
        }
    }
};

void process2(const String line) {
    log << "Processing line " << line << endl;

    Vector<int> numbers = splitString<int>(line, ',');

    State state;

    for (auto n : numbers) {
        state.add(n);
    }

    while (true) {
        state.process();

        if (state.turn == 2020) {
            log << "Part1: " << state.last << endl;
        }

        if (state.turn == 30000000) {
            log << "Part2: " << state.last << endl;
            break;
        }
    }
}

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    for (const auto &line : lines) {
        process2(line);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
