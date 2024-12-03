#include "../../common_fast.h"

struct Context {

    enum ParseState { INSTR, A, COMMA, B, END };

    template <bool useDoDont> void part(auto lines) {
        auto result{0};
        const String instr{"mul("};

        ParseState state = INSTR;
        int instrindex = 0;
        String term;
        int a;
        int b;
        bool enabled = true;

        auto reset = [&]() {
            instrindex = 0;
            state = INSTR;
            term = "";
        };

        auto validate = [&]() {
            if constexpr (useDoDont) {
                if (enabled) {
                    result += a * b;
                }
            } else {

                result += a * b;
            }
            /* log << "Found mul(" << a << ","<< b << ") -> " << result << " | " << enabled << endl; */
        };

        enabled = true;

        for (auto line : lines) {
            reset();

            for (int i = 0; i < line.size(); ++i) {
                auto c = line[i];

                /* log << c << " | " << state << endl; */

                if constexpr (useDoDont) {
                    if (i >= 4) {
                        auto str = line.substr(i - 4, 4);
                        if (str == "do()") {
                            enabled = true;
                        }
                    }
                    if (i >= 7) {
                        auto str = line.substr(i - 7, 7);
                        if (str == "don't()") {
                            enabled = false;
                        }
                    }
                }

                switch (state) {
                    case INSTR:
                        if (c == instr[instrindex]) {
                            instrindex++;
                            if (instrindex == instr.size()) {
                                state = A;
                            }
                        } else {
                            reset();
                        }
                        break;
                    case A:
                        if (std::isdigit(c)) {
                            term += c;
                        } else {
                            if (term.size() > 0) {
                                a = parseInt(term);
                                state = COMMA;
                                --i;
                            } else {
                                reset();

                                --i;
                            }
                        }
                        break;
                    case COMMA:
                        if (c == ',') {
                            state = B;
                            term = "";
                        } else {
                            reset();
                            --i;
                        }
                        break;
                    case B:
                        if (std::isdigit(c)) {
                            term += c;
                        } else {
                            if (term.size() > 0) {
                                b = parseInt(term);
                                state = END;
                                --i;
                            } else {
                                reset();
                                --i;
                            }
                        }
                        break;
                    case END:
                        if (c == ')') {
                            validate();
                            reset();
                        } else {
                            reset();
                            --i;
                        }
                        break;
                }
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0};

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
    {
        Context context;
        context.part<false>(lines);
        context.part<true>(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
