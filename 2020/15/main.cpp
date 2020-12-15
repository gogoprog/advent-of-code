#include "../../common.h"

struct State {
    Map<int, int> preLastSpoken;
    int turn{0};
    int last;

    inline void add(const int value) {
        preLastSpoken[last] = turn;
        turn++;
        last = value;
    }

    inline void process() {
        auto lastv = preLastSpoken[last];
        if (preLastSpoken[last] == 0) {
            add(0);
        } else {
            auto v = turn - lastv;
            add(v);
        }
    }
};

void process2(const String line) {
    log << "Processing line " << line << endl;

    Vector<int> numbers = splitString<int>(line, ',');
    int len = numbers.size();

    {
        State state;

        for (auto n : numbers) {
            state.add(n);
        }

        for (int i{len}; i < 2020; ++i) {
            state.process();
        }

        log << "Part1: " << state.last << endl;
    }
    {
        State state;

        for (auto n : numbers) {
            state.add(n);
        }

        for (int i{len}; i < 30000000; ++i) {
            state.process();
        }

        log << "Part2: " << state.last << endl;
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
