#include "../../common.h"

using Registers = Array<int, 4>;

class Context;

struct State {
    Registers registers;
    int cursor{0};
    /* Vector<bool> toggledLines; */
    int toggleLine = -1;
};

struct Arg {
    Arg() = default;

    Arg(const String input) {
        char c = input[0];
        if (c >= 'a' && c <= 'd') {
            v = c - 'a';
            type = Type::register_;
        } else {
            type = Type::value;
            v = std::stoi(input);
        }
    }

    enum class Type { unset, register_, value };
    Type type{Type::unset};
    int v;

    int &get(State &state) {
        switch (type) {
            case Type::register_:
                return state.registers[v];
            case Type::value:
                return v;
        }
        throw "No way";
    }

    int get(State &state) const {
        switch (type) {
            case Type::register_:
                return state.registers[v];
            case Type::value:
                return v;
        }
        throw "No way";
    }
};

using Func = Function<void(State &, Arg, Arg)>;

struct Line {
    String strInstr;
    Func instruction;
    Arg a;
    Arg b;
};

using Program = Vector<Line>;

Map<String, Func> funcs{
    {"cpy", [](auto &state, auto a, auto b) { b.get(state) = a.get(state); }},
    {"inc", [](auto &state, auto a, auto b) { a.get(state)++; }},
    {"dec", [](auto &state, auto a, auto b) { a.get(state)--; }},
    {"jnz",
     [](auto &state, auto a, auto b) {
         if (a.get(state)) {
             state.cursor += b.get(state) - 1;
         }
     }},
    {"tgl",
     [](auto &state, auto a, auto b) {
         int targetLine = state.cursor + a.get(state);
         state.toggleLine = targetLine;
     }},
};

Map<String, String> toggleMap{
    {"cpy", "jnz"}, {"inc", "dec"}, {"dec", "inc"}, {"jnz", "cpy"}, {"tgl", "inc"},
};

bool isLetter(String a) {
    return a[0] >= 'a' && a[0] <= 'd';
}

struct Context {
    Program program;
    State state;

    void parse(const String line) {

        String i, a, b;
        InputStringStream iss{line};
        iss >> i >> a >> b;

        Line pgmLine{i};

        pgmLine.instruction = funcs[i];

        pgmLine.a = Arg(a);

        if (b.size()) {
            pgmLine.b = Arg(b);
        }

        program.push_back(pgmLine);
    }

    void run(const int a = 0) {
        std::fill(state.registers.begin(), state.registers.end(), 0);
        state.cursor = 0;
        state.registers[0] = a;

        while (state.cursor < program.size()) {
            const auto &line = program[state.cursor];

            state.toggleLine = -1;

            line.instruction(state, line.a, line.b);

            if (state.toggleLine >= 0 && state.toggleLine < program.size()) {
                auto &alteredLine = program[state.toggleLine];
                alteredLine.strInstr = toggleMap[alteredLine.strInstr];
                alteredLine.instruction = funcs[alteredLine.strInstr];
            }

            state.cursor++;
        }

        log << "a = " << state.registers[0] << endl;
    }
};

void process(const String filename, const int a = 0) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx{};

    for (auto &line : lines) {
        ctx.parse(line);
    }

    ctx.run(a);
}

int main() {
    process("sample.txt");
    process("input12.txt");
    process("input.txt", 7);
    process("input.txt", 12);
    return 0;
}
