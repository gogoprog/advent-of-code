#include "../../common.h"

using Registers = Array<int, 4>;

class Context;

struct State {
    Registers registers;
    int cursor{0};
};
using Func = Function<void(State &, int, int)>;

struct Line {
    Func instruction;
    int a;
    int b;
};

using Program = Vector<Line>;

Map<String, Func> funcs{
    {"cpy", [](auto &state, auto a, auto b) { state.registers[b] = a; }},
    {"cpyr", [](auto &state, auto a, auto b) { state.registers[b] = state.registers[a]; }},
    {"inc",
     [](auto &state, auto a, auto b) {
         state.registers[a]++;
     }},
    {"dec", [](auto &state, auto a, auto b) { state.registers[a]--; }},
    {"jnz",
     [](auto &state, auto a, auto b) {
         if (state.registers[a]) {
             state.cursor += b - 1;
         }
     }},
};

bool isLetter(String a) { return a[0] >= 'a' && a[0] <= 'd'; }

struct Context {
    Program program;
    State state;

    void parse(const String line) {

        String i, a, b;
        InputStringStream iss{line};
        iss >> i >> a >> b;

        Line pgmLine;

        if (i == "cpy" && isLetter(a)) {
            pgmLine.instruction = funcs["cpyr"];
        } else {
            pgmLine.instruction = funcs[i];
        }

        if (isLetter(a)) {
            pgmLine.a = a[0] - 'a';
        } else {
            pgmLine.a = std::stoi(a);
        }

        if (b.size()) {
            if (isLetter(b)) {
                pgmLine.b = b[0] - 'a';
            } else {
                pgmLine.b = std::stoi(b);
            }
        }

        program.push_back(pgmLine);
    }

    void run(const int c = 0) {
        std::fill(state.registers.begin(), state.registers.end(), 0);
        state.cursor = 0;
        state.registers[2] = c;

        while (state.cursor < program.size()) {
            auto &line = program[state.cursor];

            line.instruction(state, line.a, line.b);

            state.cursor++;
        }

        log << "a = " << state.registers[0] << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx;

    for (auto line : lines) {
        ctx.parse(line);
    }

    ctx.run();

    ctx.run(1);
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
