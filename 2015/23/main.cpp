#include "../../common.h"

struct Context;
using Registers = Vector<unsigned int>;
using Func = std::function<void(Context &ctx, int a, int b)>;

struct Line2 {
    Func func;
    int a;
    int b;
};

struct Context {
    Registers registers;
    Vector<Line2> lines;
    int cursor{0};

    void run() {
        registers = {0, 0};

        while (cursor < lines.size()) {
            auto &line = lines[cursor];
            line.func(*this, line.a, line.b);
        }

        log << "a is " << registers[0] << endl;
        log << "b is " << registers[1] << endl;
    }

    inline void jump(const int offset) { cursor += offset; }
    inline void next() { ++cursor; }
};

Map<String, Func> funcs{
    {"hlf",
     [](auto &ctx, auto r, auto b) {
         ctx.registers[r] /= 2;
         ctx.next();
     }},
    {"tpl",
     [](auto &ctx, auto r, auto b) {
         ctx.registers[r] *= 3;
         ctx.next();
     }},
    {"inc",
     [](auto &ctx, auto r, auto b) {
         ctx.registers[r]++;
         ctx.next();
     }},
    {"jmp", [](auto &ctx, auto o, auto b) { ctx.jump(o); }},
    {"jie",
     [](auto &ctx, auto r, auto o) {
         if ((ctx.registers[r] & 1) == 0) {
             ctx.jump(o);
         } else {
             ctx.next();
         }
     }},
    {"jio",
     [](auto &ctx, auto r, auto o) {
         if (ctx.registers[r] == 1) {
             ctx.jump(o);
         } else {
             ctx.next();
         }
     }},
};

void process(const String filename) {
    auto lines = getFileLines(filename);
    Context ctx;

    for (auto &line : lines) {
        InputStringStream iss{line};
        String instr;
        char c;
        ctx.lines.push_back({});
        auto &pgmLine = ctx.lines.back();

        iss >> instr;

        iss >> c;

        pgmLine.func = funcs[instr];

        switch (c) {
        case 'a':
            pgmLine.a = 0;
            break;
        case 'b':
            pgmLine.a = 1;
            break;
        case '+':
            iss >> pgmLine.a;
            break;
        case '-':
            iss >> pgmLine.a;
            pgmLine.a = -pgmLine.a;
            break;
        }

        iss >> c;

        iss >> c;
        switch (c) {
        case '+':
            iss >> pgmLine.b;
            break;
        case '-':
            iss >> pgmLine.b;
            pgmLine.b = -pgmLine.b;
            break;
        }
    }

    ctx.run();
}

inline void hlf(unsigned int &r) { r /= 2; }

inline void tpl(unsigned int &r) { r *= 3; }

inline void inc(unsigned int &r) { ++r; }

inline bool jie(unsigned int r) { return r & 1 == 0; }

inline bool jio(unsigned int r) { return r == 1; }

void direct() {
    unsigned int a{1};
    unsigned int b{0};
    if (jio(a))
        goto l18;
    inc(a);
    tpl(a);
    inc(a);
    tpl(a);
    tpl(a);
    tpl(a);
    inc(a);
    tpl(a);
    inc(a);
    tpl(a);
    inc(a);
    inc(a);
    tpl(a);
    tpl(a);
    tpl(a);
    inc(a);
    goto l40;
l18:
    tpl(a);
    inc(a);
    tpl(a);
    inc(a);
    inc(a);
    tpl(a);
    inc(a);
    tpl(a);
    inc(a);
    inc(a);
    tpl(a);
    tpl(a);
    inc(a);
    inc(a);
    tpl(a);
    inc(a);
    inc(a);
    tpl(a);
    inc(a);
    inc(a);
    tpl(a);
    log << a << endl;
l40:
    if (jio(a))
        goto l48;
    inc(b);
    if (jie(a))
        goto l46;
    tpl(a);
    inc(a);
    goto l40; // jmp + 2
l46:
    hlf(a);
    goto l40;

l48:
    log << "end" << endl;
    log << "a is " << a << endl;
    log << "b is " << b << endl;
}

void simplified(const ull input) {
    ull a{input};
    ull b{0};

    while(a != 1) {
        ++b;
        if ((a & 1) == 0 ){
            a /= 2;
        }
        else {
            a *= 3;
            ++a;
        }
    }

    log << "end" << endl;
    log << "a is " << a << endl;
    log << "b is " << b << endl;
}

int main() {
    /* process("sample.txt"); */
    /* process("input.txt"); */
    /* direct(); */

    log << "part1" << endl;
    simplified(26623);
    log << "part2" << endl;
    simplified(31911);

    return 0;
}
