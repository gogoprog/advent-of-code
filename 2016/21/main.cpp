#include "../../common.h"

enum class OpCode { swapPosition, swapLetter, rotate, rotateBasedOnPosition, reverse, move };

struct Instruction {
    OpCode opCode;
    char x;
    char y;
};

struct Context {
    Vector<Instruction> instructions;

    void parse(String line) {
        String first, second, str;
        InputStringStream iss{line};
        iss >> first >> second;

        instructions.push_back({});
        auto &instr = instructions.back();

        if (first == "swap") {
            if (second == "position") {
                instr.opCode = OpCode::swapPosition;
                iss >> instr.x >> str >> str >> instr.y;
                instr.x -= '0';
                instr.y -= '0';
            } else if (second == "letter") {
                instr.opCode = OpCode::swapLetter;
                iss >> instr.x >> str >> str >> instr.y;
            } else {
                /* throw "Impossible"; */
                assert(false);
            }
        } else if (first == "rotate") {
            if (second == "left") {
                instr.opCode = OpCode::rotate;
                instr.y = -1;
                iss >> instr.x;
                instr.x -= '0';
            } else if (second == "right") {
                instr.opCode = OpCode::rotate;
                instr.y = 1;
                iss >> instr.x;
                instr.x -= '0';
            } else if (second == "based") {
                instr.opCode = OpCode::rotateBasedOnPosition;
                iss >> str >> str >> str >> str >> instr.x;
            } else {
                /* throw "Impossible"; */
                assert(false);
            }
        } else if (first == "reverse") {
            instr.opCode = OpCode::reverse;
            iss >> instr.x >> str >> instr.y;
            instr.x -= '0';
            instr.y -= '0';
        } else if (first == "move") {
            instr.opCode = OpCode::move;
            iss >> instr.x >> str >> str >> instr.y;
            instr.x -= '0';
            instr.y -= '0';
        } else {
            /* throw "Impossible"; */
            assert(false);
        }
    }

    String scramble(const String input) {
        String w{input};
        for (auto &instr : instructions) {
            switch (instr.opCode) {
                case OpCode::swapPosition: {
                    std::swap(w[instr.x], w[instr.y]);
                } break;
                case OpCode::swapLetter: {
                    auto a = w.find(instr.x);
                    auto b = w.find(instr.y);
                    std::swap(w[a], w[b]);
                } break;
                case OpCode::rotate: {
                    if (instr.y == -1) {
                        std::rotate(w.begin(), w.begin() + instr.x, w.end());
                    } else {
                        std::rotate(w.rbegin(), w.rbegin() + instr.x, w.rend());
                    }
                } break;
                case OpCode::rotateBasedOnPosition: {
                    auto p = w.find(instr.x);
                    if (p >= 4)
                        ++p;

                    ++p;
                    p %= w.length();
                    std::rotate(w.rbegin(), w.rbegin() + p, w.rend());
                } break;
                case OpCode::reverse: {
                    std::reverse(w.begin() + instr.x, w.begin() + instr.y + 1);
                } break;
                case OpCode::move: {
                    char c = w[instr.x];
                    w[instr.x] = '.';
                    w.insert(w.begin() + instr.y + (instr.x > instr.y ? 0 : 1), c);
                    w.erase(w.begin() + w.find('.'));
                } break;
            }
        }

        return w;
    }
};

void process(const String filename, const String input, const String input2 = "") {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx{};

    for (auto &line : lines) {
        ctx.parse(line);
    }

    {
        log << "part1:" << endl;
        log << ctx.scramble(input) << endl;
    }

    if (input2.length()) {
        log << "part2:" << endl;

        String w{input};

        do {
            auto r = ctx.scramble(w);
            if (r == input2) {
                log << w << endl;
                break;
            }
        } while (std::next_permutation(w.begin(), w.end()));
    }
}

int main() {
    process("sample.txt", "abcde");
    process("input.txt", "abcdefgh", "fbgdceah");
    return 0;
}
