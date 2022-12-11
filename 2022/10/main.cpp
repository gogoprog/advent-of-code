#include "../../common.h"

enum Instruction { ADDX, NOOP };

struct PLine {
    Instruction instruction;
    int parameter;
};

struct Machine {
    Vector<PLine> program;
    Vector<int> cycles;
    int x{1};
    int addx{0};
    int cursor{0};

    void load(const Vector<String> &lines) {
        String str;

        for (auto &line : lines) {
            program.push_back({});
            auto &program_line = program.back();
            InputStringStream iss{line};

            iss >> str;
            if (str == "addx") {
                program_line.instruction = ADDX;
                iss >> program_line.parameter;
            } else {
                program_line.instruction = NOOP;
            }

            cycles.push_back(0);
        }
    }

    void update() {
        const auto &current_line = program[cursor];
        auto &current_cycles = cycles[cursor];

        x += addx;
        addx = 0;

        current_cycles++;

        switch (current_line.instruction) {
            case NOOP: {
                current_cycles = 0;
                cursor++;
            } break;

            case ADDX: {
                if (current_cycles == 2) {
                    current_cycles = 0;
                    cursor++;

                    addx = current_line.parameter;
                }

            } break;
        }
    }

    void init() {
        for (auto &c : cycles) {
            c = 0;
        }

        cursor = 0;
        x = 1;
    }

    void part1() {
        init();

        Queue<int> q({20, 60, 100, 140, 180, 220});
        int i{0};

        int result{0};

        while (!q.empty()) {
            update();

            i++;
            if (i == q.front()) {
                q.pop();
                result += i * x;
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        init();
        String crt;

        for (auto i : rs::iota_view(0, 240)) {
            update();
            int col = i % 40;

            if (abs(col - x) <= 1) {
                crt += '.';
            } else {
                crt += '#';
            }
        }

        log << "Part2: " << endl;

        for (auto y : rs::iota_view(0, 6)) {
            for (auto x : rs::iota_view(0, 40)) {
                log << crt[y * 40 + x];
            }
            log << endl;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Machine machine;
    machine.load(lines);
    machine.part1();
    machine.part2();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
