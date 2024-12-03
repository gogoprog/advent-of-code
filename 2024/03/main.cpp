#include "../../common_fast.h"

struct Context {

    enum ParseState { UNKNOWN, NEED_DIGIT, NEED_DIGIT_OR_CLOSE, NEED_DIGIT_OR_CLOSE_OR_COMMA };

    struct Instruction {
        Function<void(Vector<int> &)> func;
        int argsCount;
    };

    struct ParseResult {
        Vector<int> args;
        StringView line;
        int startIndex;
        int endIndex;
        bool valid{false};
    };

    struct Line {
        Instruction instruction;
        Vector<int> args;
    };

    static Vector<ParseResult> parseLine(const StringView &line) {
        Vector<ParseResult> results;

        ParseResult current_result;
        ParseState state = UNKNOWN;
        String term;
        bool enabled = true;

        auto reset = [&]() {
            state = UNKNOWN;
            term = "";
            current_result = {};
        };

        reset();

        for (int i = 0; i < line.size(); ++i) {
            auto c = line[i];

            switch (c) {
                case '(':
                    reset();
                    state = NEED_DIGIT_OR_CLOSE;
                    current_result.startIndex = i;
                    break;

                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':

                    switch (state) {
                        case NEED_DIGIT:
                        case NEED_DIGIT_OR_CLOSE:
                        case NEED_DIGIT_OR_CLOSE_OR_COMMA:
                            term += c;
                            state = NEED_DIGIT_OR_CLOSE_OR_COMMA;
                            break;
                        default:
                            reset();
                            break;
                    }

                    break;

                case ')':
                    switch (state) {
                        case NEED_DIGIT_OR_CLOSE:
                        case NEED_DIGIT_OR_CLOSE_OR_COMMA:
                            if (term != "") {
                                current_result.args.push_back(parseInt(term));
                            }
                            current_result.endIndex = i;
                            current_result.line = line;
                            results.push_back(current_result);
                            reset();
                            break;
                        default:
                            break;
                    }
                    reset();
                    break;

                case ',':
                    switch (state) {
                        case NEED_DIGIT_OR_CLOSE_OR_COMMA:
                            current_result.args.push_back(parseInt(term));
                            term = "";
                            state = NEED_DIGIT;
                            break;
                        default:
                            reset();
                            break;
                    }
                    break;
            }
        }
        return results;
    }

    template <bool useDoDont> void part(auto lines) {
        auto result{0};
        bool enabled{true};
        Map<String, Instruction> instructions;

        instructions["mul"] = {[&](const Vector<int> &args) {
                                   if (enabled) {
                                       result += args[0] * args[1];
                                   }
                               },
                               2};

        if constexpr (useDoDont) {
            instructions["do"] = {[&](const Vector<int> &args) { enabled = true; }, 0};
            instructions["don't"] = {[&](const Vector<int> &args) { enabled = false; }, 0};
        }

        auto resolve = [&](const auto &result) {
            for (auto &kv : instructions) {
                auto name = kv.first;

                auto len = name.length();

                if (result.startIndex >= len) {
                    auto str = result.line.substr(result.startIndex - len, len);

                    if (str == name) {
                        return Line{kv.second, result.args};
                    }
                }
            }

            return Line{};
        };

        auto valid = [&](const auto &result) {
            if (!result.instruction.func) {
                return false;
            }

            return result.args.size() == result.instruction.argsCount;
        };

        auto program = lines | rv::transform(parseLine) | rv::join | rv::transform(resolve) | rv::filter(valid);

        for (auto line : program) {
            line.instruction.func(line.args);
        }

        log << "Result: " << result << endl;
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
