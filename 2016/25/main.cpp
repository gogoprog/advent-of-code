#include "../../common.h"

using Registers = Array<int, 4>;

class Context;

struct State {
    Registers registers;
    int cursor{0};
    int toggleLine = -1;
    int transmit{-1};
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
        assert(false);
    }

    int get(State &state) const {
        switch (type) {
            case Type::register_:
                return state.registers[v];
            case Type::value:
                return v;
        }
        assert(false);
    }
};

using Func = Function<void(State &, Arg, Arg)>;

struct PLine {
    String strInstr;
    Func instruction;
    Arg a;
    Arg b;
};

using Program = Vector<PLine>;

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
    {"out", [](auto &state, auto a, auto b) { state.transmit = a.get(state); }},
};

struct Context {
    Program program;
    State state;

    void parse(const String line) {

        String i, a, b;
        InputStringStream iss{line};
        iss >> i >> a >> b;

        PLine pgmLine{i};

        pgmLine.instruction = funcs[i];

        pgmLine.a = Arg(a);

        if (b.size()) {
            pgmLine.b = Arg(b);
        }

        program.push_back(pgmLine);
    }

    bool run(const int a = 0) {
        std::fill(state.registers.begin(), state.registers.end(), 0);
        state.cursor = 0;
        state.registers[0] = a;

        int lastTransmit = 1;
        int i{0};

        while (state.cursor < program.size()) {
            const auto &line = program[state.cursor];

            state.transmit = -1;
            line.instruction(state, line.a, line.b);

            if (state.transmit != -1) {
                if (state.transmit == lastTransmit) {
                    return false;
                } else {
                    lastTransmit = state.transmit;
                    ++i;
                    if (i >= 10000) {
                        return true;
                    }
                }
            }

            state.cursor++;
        }

        return true;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx{};

    for (auto &line : lines) {
        ctx.parse(line);
    }

    int i{0};
    while(!ctx.run(i)) {
        ++i;
    }

    log << "i = " << i << endl;
}

int main() {
    process("input.txt");
    return 0;
}
